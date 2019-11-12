#include <declarations_demo.hpp>
#include <iostream>

namespace {
    struct S {
        static const int sum_const(int a, int b) {
            return a + b;
        }

        static constexpr int sum_constexpr(int a, int b) {
            return a + b;
        }
    };
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

void declarations_demo(){
    std::cout << "declarations_demo()" << std::endl;
    constexpr_demo();
}
