#include <iostream_demo.hpp>
#include <iostream>

namespace iostream_demo {

    // Adjacent string concatenation (C89/C++98)
    // If two string literals are adjacent to each other with no punctuation in between, they are merged into a single string.
    void cin_demo() {
        int x, y;
        std::cout << "Enter 2 numbers (press ENTER after each): " << std::endl;
        std::cin >> x >> y;
        std::cout << "x = " << x << " y = " << y << std::endl;
    }

    void run() {
        std::cout << "iostream_demo()" << std::endl;
        cin_demo();
    }
}
