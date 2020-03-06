#include <string_streams_demo.hpp>
#include <iostream>
#include <cassert>
#include <sstream>

namespace string_streams_demo {

// string streams:
//- like io streams; io strams: we read from keyboard and write to console
//- read and write from/to string buffer
//- internally maintains the string buffer
//- 3 string stream classes:
//     std::stringstream (read/write)
//     std::istringstream (read-only)
//     std::ostringstream (write-only)
//- header <sstream> must be included
void demo() {
    int n1 {1}, n2 {2};

    // std::cout is a standard output stream (std::ostram)
    // Insert operation automatically performs type conversion (to string).
    std::cout << "Sum of " << n1 << " and " << n2 << " is " << n1 + n2 << std::endl;

    std::stringstream ss;
    ss << "Sum of " << n1 << " and " << n2 << " is " << n1 + n2 << std::endl;

    // getting the string buffer:
    std::string s = ss.str();
    std::cout << "s = " << s << std::endl;

    // writing to the string buffer:
    ss.str("This is a new content of the string buffer.");

    // use string streamn for type conversions to string:
    ss.str(""); // clear string stream buffer
    ss << 123;
    assert(ss.str() == "123");

    // since C++11
    auto s2 = std::to_string(123);
    assert(s2 == "123");

    // reading from string stream
    std::string data = "1 2 3";
    std::stringstream ss2;
    ss2.str(data);

    int n;

    // ss.fail() checks for failed bit which is set if we try to read from the empty stream.
    // This loop would print the last integer value twice.
    while(!ss2.fail()) {
        ss2 >> n; // in the last iteration this extraction does not fill n actually
        std::cout << "n = " << n << std::endl;
    }

    // if we want to reuse the same stringstream object we need to reset its error flags:
    ss2.clear();

    ss2.str(data);

    // improvement:
    while(ss2 >> n) {
        std::cout << "n = " << n << std::endl;
    }

    // converting strings to other types e.g. integer:
    // since C++11:
    int n3 = std::stoi("123");
    assert(n3 == 123);
}

void run() {
    std::cout << "string_streams_demo::run()" << std::endl;
    demo();
}

}
