#include <strings_demo.hpp>
#include <iostream>

namespace strings_demo {

    // Adjacent string concatenation (C89/C++98)
    // If two string literals are adjacent to each other with no punctuation in between, they are merged into a single string.
    void string_literal_concatenation_demo() {
        const char* s = "This " "is " " a" " string.";
        std::cout << "s = " << s << std::endl;
    }

    void run() {
        std::cout << "strings_demo()" << std::endl;
        string_literal_concatenation_demo();
    }
}
