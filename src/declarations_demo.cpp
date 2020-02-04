#include <declarations_demo.hpp>
#include <cassert>
#include <iostream>

namespace declarations_demo {
struct S {
    static const int sum_const(int a, int b) {
        return a + b;
    }

    static constexpr int sum_constexpr(int a, int b) {
        return a + b;
    }
};

// 0! is 1.
constexpr int factorial(const unsigned int n) {
    return n == 0 ? 1 : n * factorial(n - 1);
}

// Fibonacci sequence: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, ...
constexpr int fibonacci(const unsigned int n) {
    return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n -2);
}

void constexpr_demo() {
    int n1, n2;
    n1 = n2 = 3;

    auto sum1 = S::sum_const(1, 2);

    // error: static_assert expression is not an integral constant expression
    // static_assert(sum1 == 3, "S::sum_const(1, 2)");

    // error: constexpr variable 'sum11' must be initialized by a constant expression
    // note: non-constexpr function 'sum_const' cannot be used in a constant expression
    // constexpr auto sum11 = S::sum_const(1, 2);

    auto sum2 = S::sum_const(n1, n2);

    // error: static_assert expression is not an integral constant expression
    // static_assert(sum2 == 3, "S::sum_const(n1, n2)");

    auto sum31 = S::sum_constexpr(1, 2);
    const auto sum32 = S::sum_constexpr(1, 2);
    constexpr auto sum33 = S::sum_constexpr(1, 2);

    auto sum41 = S::sum_constexpr(n1, n2);
    const auto sum42 = S::sum_constexpr(n1, n2);

    // error: constexpr variable 'sum43' must be initialized by a constant expression
    // note: read of non-const variable 'n1' is not allowed in a constant expression
    // constexpr auto sum43 = S::sum_constexpr(n1, n2);
}

void factorial_demo() {
    assert(factorial(0) == 1);
    assert(factorial(1) == 1);
    assert(factorial(2) == 2);
    assert(factorial(3) == 6);

    const int n1 = factorial(7);
    constexpr int n2 = factorial(8);
}

void fibonacci_demo() {
    assert(fibonacci(0) == 0);
    assert(fibonacci(1) == 1);
    assert(fibonacci(2) == 1);
    assert(fibonacci(3) == 2);
    assert(fibonacci(4) == 3);
    assert(fibonacci(5) == 5);
    assert(fibonacci(6) == 8);
    assert(fibonacci(7) == 13);
    assert(fibonacci(8) == 21);
    assert(fibonacci(9) == 34);
    assert(fibonacci(10) == 55);

    // The following line causes error:
    // cpp-demo: /usr/src/cpp-demo/src/declarations_demo.cpp:78: void fibonacci_demo(): Assertion `fibonacci(10) == 56' failed.
    // assert(fibonacci(10) == 56);

    const int n1 = fibonacci(11);
    constexpr int n2 = fibonacci(11);
}

void run(){
    std::cout << "declarations_demo::run()" << std::endl;
    constexpr_demo();
    factorial_demo();
    fibonacci_demo();
}

}
