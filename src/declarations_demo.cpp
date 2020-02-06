#include <declarations_demo.hpp>
#include <cassert>
#include <iostream>
#include <typeinfo>

namespace {
    // visible only in current cpp file
    void internal_function(){}
}

namespace A {
    void func_a(){}
    namespace B {
        void func_ab(){}
    }
}

namespace declarations_demo {
struct S {
    static const int sum_const(int a, int b) {
        return a + b;
    }

    static constexpr int sum_constexpr(int a, int b) {
        return a + b;
    }
};

void const_demo() {
    // const variable must be initialized.
    // error: uninitialized const ‘n1’
    // const int n1;
    // n1 = 0;
    const int n1 = 0;

    // const variable cannot be re-assigned any other value.
    // error: assignment of read-only variable ‘n1’
    // n1 = 1;

    // 3.141f is not a magic number if it's assigned to a const variable which name describes its nature/meaning.
    const float Pi { 3.141f };
}

void const_with_ptrs_and_refs_demo() {
    int x = 5;
    const int MAX = 12;
    int &ref_x1 = x;
    const int& ref_x2 = x;

    // Try to modify the pointer (e.g. ptr1) and the pointee value (*ptr1)
    const int *ptr1 = &x ;
    int* const ptr2 = &x ;
    const int * const ptr3 = &x ;

    // Find which declarations are valid. If invalid, correct the declaration
    // const int *ptr3 = &MAX ;

    // error: invalid conversion from ‘const int*’ to ‘int*’ [-fpermissive]
    // int* ptr4 = &MAX;
    const int* ptr4 = &MAX;

    const int &r1 = ref_x1 ;

    // error: binding reference of type ‘int&’ to ‘const int’ discards qualifiers
    // int& r2 = ref_x2;
    const int& r2 = ref_x2;

    // error: invalid conversion from ‘const int*’ to ‘int*’ [-fpermissive]
    // int*& p_ref1 = ptr1;
    const int*& p_ref1 = ptr1;

    // error: cannot bind non-const lvalue reference of type ‘const int*&’ to an rvalue of type ‘const int*’
    // const int*& p_ref2 = ptr2;

    // error: binding reference of type ‘int*&’ to ‘int* const’ discards qualifiers
    // int*& p_ref2 = ptr2;

    int* const& p_ref2 = ptr2;
}

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

void auto_demo() {

    // For variables, 'auto' specifies that the type of the variable that is being declared
    // will be automatically deduced from its initializer.

    // When using 'auto', initializer must be used.
    // error: declaration of ‘auto n1’ has no initializer
    // auto n1;

    auto n1 = 1;
    auto n2 = 2;

    // intializer can be expression
    auto n3 = n1 + n2;

    // if types in the expression are not matching, the type of lvalue
    // will be the one which can accomodate all values (float in this example)
    auto sumOfIntAndFloat = 3 + 3.14f;
    std::cout << "typeid(sumOfIntAndFloat).name() = " << typeid(sumOfIntAndFloat).name() << std::endl; // f
     std::cout << "sumOfIntAndFloat = " << sumOfIntAndFloat << std::endl; // 6.14

    // the true value in using 'auto' comes when using it with lambdas and templates
    auto sum = [](int a, int b) {
        return a + b;
    };

    // initializer can be a function call
    auto fnResVal = sum(3, 4);
    std::cout << "typeid(fnResVal).name() = " << typeid(fnResVal).name() << std::endl; // i
    std::cout << "fnResVal = " << fnResVal << std::endl; // 7

    static auto n4 = 4;

    //
    // auto l = r;
    // auto discards const if r is const non-ref or ref type
    //

    const int n5 = 5;

    // WARNING: const is discarded!
    // an5 is of type int
    auto an5 = n5;
    an5 = 55;
    assert(an5 == 55);

    // n5 is  const int
    // an6 is const int
    const auto an6 = n5;
    // error: assignment of read-only variable ‘an6’
    // an6 = 66;

    const int& rn5 = n5;

    // WARNING: const is discarded and type is not a reference!
    // rn5 is const int&
    // rn6 is       int
    auto rn6 = rn5;
    rn6 = 6;
    std::cout << "typeid(rn6).name() = " << typeid(rn6).name() << std::endl; // i
    std::cout << "rn6 = " << fnResVal << std::endl; // 6

    // Use auto& to make variable to be a reference.
    // n1 is          int
    // ar1 is of type int&
    auto& ar1 = n1;

    //
    // auto& l = r;
    // auto& preserves const if r is const non-ref or ref type
    //

    // n5 is  const int
    // ar2 is const int&
    auto& ar2 = n5;

    // rn5 is const int&
    // ar3 is const int&
    auto& ar3 = rn5;

    // n6 is int
    auto n6 {1};

    // error: direct-list-initialization of ‘auto’ requires exactly one element [-fpermissive]
    // auto n7 {1, 2};

    // n7 type is std::initializer_list<int>
    auto n7 = {1, 2};
}

void namespace_demo(){
    // content of the anonymous namespace can be accessed in the same cpp file without need to use any scoping qualifiers
    internal_function();

    using namespace A::B;
    func_ab();

    {
        using namespace A;
        B::func_ab();
    }

    A::B::func_ab();
}

void run(){
    std::cout << "declarations_demo::run()" << std::endl;
    auto_demo();
    const_demo();
    const_with_ptrs_and_refs_demo();
    constexpr_demo();
    factorial_demo();
    fibonacci_demo();
    namespace_demo();
}

}
