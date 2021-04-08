#include <std_string_view_demo.hpp>
#include <iostream>

namespace std_string_view_demo {


void demo() {
    std::string_view sw("test");
    std::cout << sw << std::endl;
}

void run() {
    std::cout << "std_string_view_demo::run()" << std::endl;
    demo();
}

}
