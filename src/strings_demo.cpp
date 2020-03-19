#include <strings_demo.hpp>
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace strings_demo {

namespace c_strings_demo {

const char* combine(const char* name, const char* surname) {
    // warning: address of local variable ‘combined’ returned [-Wreturn-local-addr]
    char combined[20];

    strcpy(combined, name);
    strcat(combined, " ");
    strcat(combined, surname);
    return combined;
}

const char* combine2(const char* name, const char* surname) {
    // Problem: buffer overflow as strlen does not take into account null-terminator character ('\0' , called NUL in ASCII)
    char* combined = new char[strlen(name) + strlen(" ") + strlen(surname)];

    strcpy(combined, name);
    strcat(combined, " ");
    strcat(combined, surname);
    return combined;
}

const char* combine3(const char* name, const char* surname) {
    // fixed buffer size: + 1 for '\0'
    char* combined = new char[strlen(name) + strlen(" ") + strlen(surname) + 1];

    strcpy(combined, name);
    strcat(combined, " ");
    strcat(combined, surname);
    return combined;
}

void show_problems() {

    // Problem: we assume the length of input strings
    char name[10];
    char surname[10];
    std::cout << "Enter your name: " << std::endl;
    std::cin.getline(name, 10);
    std::cout << "Enter your surname: " << std::endl;
    std::cin.getline(surname, 10);
    const char* res = combine(name, surname);

    // Problem: does not print anything (or some garbage value).
    // Also, if compiled with g++ it silently crashes.
    // std::cout << "Combined = \"" << res << "\"" << std::endl;

    const char* res2 = combine2(name, surname);

    // prints expected value in form "<name> <surname>"
    std::cout << "Combined = \"" << res2 << "\"" << std::endl;

    // Problem, we might forget to delete this pointer (and to use delete[] operator...)
    delete[] res2;
}

} // namespace c_strings_demo


// Adjacent string concatenation (C89/C++98)
// If two string literals are adjacent to each other with no punctuation in between, they are merged into a single string.
void string_literal_concatenation_demo() {
    const char* s = "This " "is " " a" " string.";
    std::cout << "s = " << s << std::endl;
}

// C++ provides std::string class which:
// - wraps raw string without need to deal with memory allocation and dallocation - all is done automatically!
// - resides in <string> header
namespace std_string_demo {

void demo() {
    // Initialize and assign

    std::string s = "hello";
    s = "Hello";

    // Access each character:
    assert(s[0] == 'H');

    // Changing the character (via overloaded subscript operator)
    s[0] = 'B';
    auto ch1 = s[1];
    assert(ch1 == 'e');

    // Print string in terminal:
    std::cout << "s = " << s << std::endl;

    // Filling the string from terminal:
    std::cout << "Type in a new value for s (but watch for SPACE characters...) = " << std::endl;
    // This stops reading on the first SPACE or ENTER character.
    // istream extraction operations use whitespaces as separators; Therefore, this operation will only extract what can be considered a word from the stream.
    // When user presses ENTER key, this adds newline character ('\n') to the stream.
    std::cin >> s;
    std::cout << "s = " << s << std::endl;

    // empty std::cin (read that newline character)
    std::string temp;
    // Extracts characters from input stream and stores them into string until newline character, '\n' is found.
    // IMPORTANT: If the delimiter ('\n') is found, it is extracted and discarded (i.e. it is not stored and the next input operation will begin after it).
    std::getline(std::cin, temp);
    assert(temp.length() == 0);
    // assert(temp[0] == '\n');

    // To read entire line use:
    std::cout << "Type in a new value for s (try typing SPACE characters) = " << std::endl;

    std::getline(std::cin, s);
    std::cout << "s = " << s << std::endl;

    // Get string size:
    std::cout << "s length = " << s.length() << std::endl;
    // std::string caches string length so std::string's length() is more efficient than C-style len().

    // Concatenate
    std::string s2{"Gnocco"};
    std::string s3{s + " " + s2};
    s3 += " is my fav dish.";
    std::cout << "s3 = " << s3 << std::endl;

    // Using auto keyword:
    // 'auto' resolves to the simplest possible type which "works" for the type of the assignment. The compiler does not "look forward to see what you are doing with the variable".
    // error: request for member ‘insert’ in ‘s4’, which is of non-class type ‘const char*’
    // auto s4{"Hello world!"};

    // error: unable to find string literal operator ‘operator""s’ with ‘const char [13]’, ‘long unsigned int’ arguments
    // error: unable to deduce ‘auto’ from ‘<expression error>’
    // auto s4{"Hello world!"s};

    // From C++14 onwards, if using namespace std::string_literals; is used:
    using namespace std::string_literals;
    auto s4 = "Hello world!"s;

    // Search
    // 0 - where to start the search from
    auto i = s4.find("world", 0);
    assert(i == 6);

    auto i2 = s4.find("string_that_is_not_a_substring", 0);
    // std::string::npos has value -1
    assert(i2 == std::string::npos);

    // Insert string at position:
    s4.insert(i, "beautiful ");
    assert(s4 == "Hello beautiful world!");

    auto s5 = "ABCD"s;
    auto s6 = "BBCD"s;

    if (s5 < s6) {
        std::cout << s5 << " < " << s6 << std::endl;
    }

    // Remove characters:
    // erase first 2 characters
    s5.erase(0, 2);
    assert(s5 == "CD");

    s5.erase();
    assert(s5 == "");

    s6.clear();
    assert(s6 == "");

    // get C-style string
    // error: invalid conversion from ‘const char*’ to ‘char*’ [-fpermissive]
    // char* p_ch = s4.c_str();
    const char* p_ch = s4.c_str();
    printf("%s", p_ch);
}

std::string combine(const std::string& name, const std::string& surname) {
    return name + " " +surname;
}

void std_string_combine_demo() {
    std::string name;
    std::string surname;
    std::cout << "Enter your name: " << std::endl;
    std::getline(std::cin, name);
    std::cout << "Enter your surname: " << std::endl;
    std::getline(std::cin, surname);
    auto res = combine(name, surname);
    std::cout << "res = " << res << std::endl;
}

std::string ToUpper(const std::string &str){
    // std::cout << "ToUpper(const std::string &str)" << std::endl;

    std::string s(str.length(), ' ');

    for(auto i = 0; i < str.length(); ++i) {
        s[i] = toupper(str[i]);
    }

    return s;
};

std::string ToLower(const std::string &str) {
    // std::cout << "ToLower(const std::string &str)" << std::endl;
    std::string s(str.length(), ' ');

    for(auto i = 0; i < str.length(); ++i) {
        s[i] = tolower(str[i]);
    }

    return s;
};

// In-place string changing
void ToUpper(std::string &str){
    // std::cout << "ToUpper(std::string &str)" << std::endl;
    for(auto i = 0; i < str.length(); ++i) {
        str[i] = toupper(str[i]);
    }
};

// In-place string changing
void ToLower(std::string &str) {
    // std::cout << "ToLower(std::string &str)" << std::endl;
    for(auto i = 0; i < str.length(); ++i) {
        str[i] = tolower(str[i]);
    }
};

void test_string_conversion_functions(){
    using namespace std::string_literals;

    auto s1 = "originally, this was all in lower"s;
    std::cout << "Original = " << s1 << std::endl;
    ToUpper(s1);
    std::cout << "ToUpper = " << s1 << std::endl;

    auto s2 = "ORIGINALLY, THIS WAS ALL IN UPPER"s;
    std::cout << "Original = " << s2 << std::endl;
    ToUpper(s2);
    std::cout << "ToUpper = " << s2 << std::endl;

    const auto s3 = "originally, this was all in lower"s;
    std::cout << "Original = " << s3 << "; ToUpper = " << ToUpper(s3) << std::endl;

    const auto s4 = "ORIGINALLY, THIS WAS ALL IN UPPER"s;
    std::cout << "Original = " << s4 << "; ToLower = " << ToLower(s4) << std::endl;
}

enum class Case { SENSITIVE, INSENSITIVE };

// return position of the first character of the substring, else std::string::npos
size_t Find (
    const std::string &source,           // Source string to be searched
    const std::string &search_string,    // The string to search for
    Case searchCase = Case::INSENSITIVE, // Choose case sensitive/insensitive search
    size_t offset = 0 ) {                // Start the search from this offset
    if (searchCase == Case::SENSITIVE) {
        return source.find(search_string, offset);
    }

    auto sourceInLower = ToLower(source);
    auto searchStringInLower = ToLower(search_string);
    return sourceInLower.find(searchStringInLower, offset);
}

void test_find() {
    using namespace std::string_literals;
    auto s = "This is some SENtence"s;
    auto subStr = "sentence"s;

    auto i = Find(s, subStr);
    if (i == std::string::npos) {
        std::cout << "String \"" << s << "\" does not contain substring \"" << subStr << "\" when using case insensitive search." << std::endl;
    } else {
        std::cout << "String \"" << s << "\" does contain substring \"" << subStr << "\" at posistion " << i << " when using case insensitive search." << std::endl;
    }

    i = Find(s, subStr, Case::SENSITIVE);
    if (i == std::string::npos) {
        std::cout << "String \"" << s << "\" does not contain substring \"" << subStr << "\" when using case sensitive search." << std::endl;
    } else {
        std::cout << "String \"" << s << "\" does contain substring \"" << subStr << "\" at posistion " << i << " when using case sensitive search." << std::endl;
    }
}

// Add one more function called FindAll, that returns the starting indices of all the found substrings in a target string.
// Return the indices in a vector. Support case sensitive/insensitive search.
// Return indices of found strings, else an empty vector.
std::vector<int>
FindAll(
    const std::string &target,         //Target string to be searched
    std::string search_string,         //The string to search for
    Case searchCase = Case::INSENSITIVE,//Choose case sensitive/insensitive search
    size_t offset = 0) {                //Start the search from this offset

    std::vector<int> indices;

    auto localOffset = offset;
    while(true) {
        auto indexFound = Find(target, search_string, searchCase, localOffset);
        if (indexFound == std::string::npos) {
            break;
        } else {
            indices.push_back(static_cast<int>(indexFound));
            localOffset = indexFound + 1;
        }
    }

    return indices;
}

void test_FindAll() {
    std::cout << "test_FindAll()" << std::endl;
    std::string source{"abcdefabcdeabababcdddabc"};
    auto indices = FindAll(source, "ABC", Case::INSENSITIVE, 0);
    for(auto i : indices) {
        std::cout << i << std::endl;
    }
}

void escape_characters_demo() {
    std::string fileName{"c:\dir1\new_dir\this_is_a_file"}
    std::cout << fileName << std::endl;
}


} // namespace std_string_demo

void run() {
    std::cout << "strings_demo()" << std::endl;
    // c_strings_demo::show_problems();
    // string_literal_concatenation_demo();
    // std_string_demo::demo();
    // std_string_demo::std_string_combine_demo();
    // std_string_demo::test_string_conversion_functions();
    // std_string_demo::test_find();
    // std_string_demo::test_FindAll();
    std_string_demo::escape_characters_demo();
}

}
