#include <file_io_demo.hpp>
#include <iostream>
#include <cassert>
#include <filesystem>
#include <fstream>

// File I/O is supported via following classes:
// - ofstream - output file stream for writing
// - ifstream - input file stream for reading
// - fstream - file stream for both reading and writing
//
// - include <fstream> header
// - all classes can be used for both text & binary I/O
// - they support modes that control how the file is opened and operated
// - to check whether stream is open use: is_open()
// - stream class hierarchy:
//  ios_base (file opening modes, formatting flags)
//      istream (console input)
//          ifstream (file input)
//          istringstream (buffer input)
//
//      ostream (console output)
//          ofstream (file output)
//          ostringstream (buffer output)
//
//          iostream (console I/O)
//              fstream (file I/O)
//              stringstream (buffer I/O)
//
// File opening modes:
// - used when opening a file
// - constants defined in std::ios_base class
// - some can be combined e.d. std::ios:in | std::ios::out etc.
// Modes:
//  app     - seek to the end before each write operation (append)
//  binary  - open in binary mode
//  in      - open for reading (default for ifstream)
//  out     - open for writing (default for ofstream)
//  trunc   - discard file contents before opening (truncate)
//  ate     - seek to end after open
//
namespace file_io_demo {

// Output:
// Text before first SPACE character: This
// First line: This is some text..., int value = 10
void text_file_demo() {
    {
        // this c-tor creates a file
        std::ofstream out{"data.txt"};
        // or use open function:
        // std::ofstream out;
        // out.open("data.txt");

        out << "This is some text..." << std::endl;
        out << 10 << std::endl;
        out.close(); // this is optional as ofstream destructor calls it anyway
    }

    {
        // Open existing file via constructor.
        // ifstream will throw a failure exception if the file cannot be opened, e.g. file not found, no read permissions.
        // It will open it as read-only.
        std::ifstream in{"data.txt"};
        std::string text_before_first_space;
        in >> text_before_first_space;
        std::cout << "Text before first SPACE character: " << text_before_first_space << std::endl;
    }

    {
        std::ifstream in;
        // open via open() function
        in.open("data.txt");

        std::string first_line;
        std::getline(in, first_line);
        int value;
        in >> value;
        std::cout << "First line: " << first_line << ", int value = " << value << std::endl;
    }
}

// Stream classes have State Flags which show the state of the stream.
// We can use associated functions to check their (boolean) value.
//
// goodbit = no error;                         bool good() // use to check if operation has succeeded
// badbit  = irrecoverable stream error;       bool bad()  // e.g. corrupted disk
// failbit = I/O operation faild;              bool fail() [operator! is overloaded to check this flag]
// eofbit  = end of file reached during input; bool eof()
//
void error_handling_demo(){
    {
        std::ifstream in{"not_existing_file.txt"};
        if (!in.is_open()) {
            std::cout << "Failed to open a file." << std::endl;
        }

        if (in.fail()) {
            std::cout << "Failed to open a file." << std::endl;
        }

        // ! - stream evaluation operator (NOT)
        // http://www.cplusplus.com/reference/ios/ios/operator_not/
        // Returns true if either failbit or badbit is set, and false otherwise.
        // This is equivalent to calling member fail().
        if (!in) {
             std::cout << "Failed to open a file." << std::endl;
        }

        // stream evaluation operator
        // http://www.cplusplus.com/reference/ios/ios/operator_bool/
        if (in) {
             std::cout << "File opened successfully." << std::endl;
        }
    }

    {
        // create a file and write something in it
        std::ofstream out{"data.txt"};
        out << "This is some text..." << std::endl;
        out << 10 << std::endl;
    }

    {
        std::ifstream in{"data.txt"};
        std::string first_line;
        std::getline(in, first_line);
        int value{};
        in >> value;
        if (in.good()) {
            std::cout << "I/O operations were successful." << std::endl;
            std::cout << "First line: " << first_line << ", int value = " << value << std::endl;
        } else {
            std::cout << "Some I/O operations failed." << std::endl;
        }

        in >> value;
        if (in.fail()) {
            std::cout << "Failed to read int from a file." << std::endl;
        }

        if (in.eof()) {
            std::cout << "End Of File reached." << std::endl;
        }

        // to set (to true) specific flag:
        in.setstate(std::ios::failbit);
        // clear state flags
        // clear() internally sets the goodbit and sets to false all other bits
        in.clear();
    }

}

// Function copies content of source file into another, new file.
void copyTextFileContentDemo(){
    using namespace std::filesystem;

    // path to the current working directory
    std::cout << "current_path() = " << current_path() << std::endl;

    path pathSource(current_path());
    // https://en.cppreference.com/w/cpp/filesystem/path/append
    // /= operator inserts OS-specific separator ('/' in case of Unix)
    pathSource /= "src";
    pathSource /= "file_io_demo.cpp";
    std::cout << "pathSource = " << pathSource << std::endl;

    path pathDest(current_path());
    pathDest /= "file_io_demo (copy).cpp";

    std::ifstream inFileStream{pathSource};

    if (!inFileStream) {
        std::cout << "Failed to open file " << pathSource << std::endl;
    }

    std::ofstream outFileStream{pathDest};

    std::string line;
    while (!std::getline(inFileStream, line).eof()) {
        outFileStream << line << std::endl;
    };

    inFileStream.close();
    outFileStream.close();

}

// Streams internalluy contain pointers which point to the location where will the next I/O action take place.
// These pointers are known as:
// put pointer - in output stream
// get pointer - in input stream
void write_read_char_demo(){
    {
        std::ofstream out("out.txt");
        if (!out){
            std::cout << "Failed to open file for writing" << std::endl;
            return;
        }

        // tellp = "tell put pointer value"
        std::cout << "Current ofstream pointer is: " << out.tellp() << std::endl;

        std::string message{"Today i 7th April 2020 and the world is in the middle of Corona virus pandemic."};

        // We want to write this message into file character by character
        for (char ch : message) {
            out.put(ch);
        }

        // seek 7th position from the beginning
        out.seekp(7);
        out.put('s');

        out.close();
    }

    {
        std::ifstream in("out.txt");
        if (!in) {
            std::cout << "Failed to open file for reading" << std::endl;
            return;
        }

        // tellg = "tell get pointer value"
        // returns 0 when points to the first character
        // returns -1 on failure
        std::cout << "Current ifstream pointer is: " << in.tellg() << std::endl;

        char ch{};
        while(in.get(ch)){
            std::cout << ch;
        }
        // Stream state is EOF now.

        std::cout << std::endl;

        // If we want to read from the random character we need to move get pointer first

        // Streams maintain their error and EOF states until they are cleared EXPLICITLY.
        // seekg() does NOT clear EOF automatically. If we try to call seekg now, it would
        // return error (-1).
        // Clear the previous EOF.
        in.clear();

        // seekg overload which takes offset as an argument (new absolute position within
        // the stream (relative to the beginning)).
        in.seekg(5);
        std::cout << "Current ifstream pointer is: " << in.tellg() << std::endl;

        while(in.get(ch)){
            std::cout << ch;
        }

        std::cout << std::endl;

        in.clear();

        // second seekg() overload has 2 parameters: an offset and base position
        // base position can be:
        // std::ios::beg - seeking will start from the beginning
        // std::ios::cur - seeking will start from the current position
        // std::ios::end - seeking will start from the end

        // go 5 positions beyond the end of file
        in.seekg(5, std::ios::end);
        std::cout << "Current ifstream pointer is: " << in.tellg() << std::endl; // 85

        in.seekg(-5, std::ios::end);
        std::cout << "Current ifstream pointer is: " << in.tellg() << std::endl; // 75

        while(in.get(ch)){
            std::cout << ch;
        }
        std::cout << std::endl; // emic.

         in.clear();

        in.seekg(10, std::ios::beg);
        std::cout << "Current ifstream pointer is: " << in.tellg() << std::endl; // 10

        while(in.get(ch)){
            std::cout << ch;
        }
        std::cout << std::endl;
    }

    // Use std::fstream class to perform both Input and Output operations on the same file
    // By default it uses std::ios_base::in | std::ios_base::out modes.
    {
        // error: the type ‘const string’ {aka ‘const std::__cxx11::basic_string<char>’} of ‘constexpr’ variable ‘fileName’ is not literal
        // https://stackoverflow.com/questions/27123306/is-it-possible-to-use-stdstring-in-a-constexpr
        // before C++20: std::basic_string (and so std::string) has non-trivial destructor
        //    => it's not literal type
        //    => its instance cannot be constexpr
        // constexpr std::string fileName{"file.txt"};

        //  warning: ISO C++ forbids converting a string constant to ‘char*’ [-Wwrite-strings]
        // constexpr char* fileName = "file.txt";

        constexpr char fileName[] = "file.txt";
        std::fstream stream{fileName};
        // fstream constructor will not create file!
        if (!stream) {
            std::cout << "fstream failed to open a file. Creating file..." << std::endl;
            std::ofstream out{fileName};
            out.close();
            stream.open(fileName);
        }

        std::cout << "After opening file: " << std::endl;
        std::cout << "Current fstream put pointer is: " << stream.tellp() << std::endl;
        std::cout << "Current fstream get pointer is: " << stream.tellg() << std::endl;

        stream << "Hello, world!" << std::endl;

        std::cout << "After writing a line: " << std::endl;
        std::cout << "Current fstream put pointer is: " << stream.tellp() << std::endl;
        std::cout << "Current fstream get pointer is: " << stream.tellg() << std::endl;

        stream.seekg(0);
        std::cout << "After seekg(0): " << std::endl;
        std::cout << "Current fstream put pointer is: " << stream.tellp() << std::endl;
        std::cout << "Current fstream get pointer is: " << stream.tellg() << std::endl;

        std::string line;
        std::getline(stream, line);

        std::cout << "After reading a line: " << std::endl;
        std::cout << "Current fstream put pointer is: " << stream.tellp() << std::endl;
        std::cout << "Current fstream get pointer is: " << stream.tellg() << std::endl;

        std::cout << "Line read from the fstream: " << line << std::endl;
    }
}


void run() {
    std::cout << "file_io_demo::run()" << std::endl;
    // text_file_demo();
    // error_handling_demo();
    // copyTextFileContentDemo();
    write_read_char_demo();
}

}
