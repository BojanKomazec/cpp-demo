#include <recursion_demo.hpp>
#include <iostream>

namespace recursion_demo {

    // 5! = 5 * 4 * 3 * 2 * 1
    // 0! = 1
    unsigned int factorial(const unsigned int n) {
        if (n == 0) {
            return 1;
        }
        return n * factorial(n - 1);
    }

    void factorial_demo() {
        std::cout << "factorial(0) = " << factorial(0) << std::endl;
        std::cout << "factorial(1) = " << factorial(1) << std::endl;
        std::cout << "factorial(5) = " << factorial(5) << std::endl;
    }


    void run() {
        std::cout << "recursion_demo()" << std::endl;
        factorial_demo();
    }
}
