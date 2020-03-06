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

// constexpr = Constant Expression
// - such expression MIGHT be evaluated in compile time
// - can be applied to variable declarations or functions
// - may increase the performance of the code as computation is done at compile time
//
// When to use const and when constexpr?
// - Initialization of a const variable can be deferred until runtime;
//   constexpr variable must be initialized at compile time
// - all constexpr variables are const but not the other way round
// - use const to indicate the value cannot be modified.
// - use constexpr to create expressions that can be evaluated at compile time such
// as declarations and functions
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

int get_square(int n) {
    return n*n;
}

//
// constexpr function
//
// Return value of this function is computed/evalueated at compile time =>
//    its argument must be known/evalueted in compile time.
// If functions returns a value that can be computed at compile time then such
//    function can be a constexpr function.
//
// Such function must accept and return only literal types.
// Literal types are types allowed in constant expressions:
// - void
// - scalar types (int, float, double, char)
// - references
// - arrays
// - classes with constexpr constructors
//
// Should contain only a return statement. (only in C++11; not in C++14+)
// constexpr functions are implicitly inline (will be defined in a header file)
//
constexpr int get_square2(int n) {
    return n*n;
}

// We can have multiple statements in constexpr functions in C++14
constexpr int min(int n1, int n2) {
    if (n1 < n2) {
        return n1;
    }

    return n2;
}

void constexpr_demo_2(){
    int n1 = 1;
    int arr1[n1];

    // This const expression is evaluated in compile time.
    // Not all const expressions are evaluated in compile time.
    const int n2 = 2;
    int arr2[n2];

    // Some constants are initialized in run time.
    const int square = get_square(n1);

    // the value of n is computed at compile time
    constexpr int n3 = 2;
    // we can use n in expressions that expect compile-time constant e.g. array size
    int arr3[n3];

    // constexpr variable can be initialized only with constant expression.
    // error: call to non-constexpr function ‘int declarations_demo::get_square(int)’
    // constexpr int square2 = get_square(n1);

    // error: the value of ‘n1’ is not usable in a constant expression
    // constexpr int square2 = get_square2(n1);

    constexpr int square2 = get_square2(5);

    // constexpr function can be used to initialize non-const variables.
    // Here it behaves as a normal function - it's executed in runtime and its return
    // value is computed in runtime.
    int n4 = get_square2(5);

    int n5 = min(3, 5);
    assert(n5 == 3);
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

// returns Rvalue
// (n1 + n2) is a temporary value.
// We cannot assign a value to it.
int add(int n1, int n2) {
    return n1 + n2;
}

// returns Lvalue
// Function returns n, which is an alias of the value that persists before and after function call
// We can assign a value to it.
int& increment(int& n) {
    n += 1;
    return n;
}

int Square(int&& x){
    return x*x;
}

// Lvalue:
// - has a name
// - all variables are Lvalues
// - can be assigned a value
// - some expressions return Lvalue
// - Lvalue persists beyond the expression
// - functions that return by reference return Lvalue
// - it is possible to create a reference to it ("Lvalue reference"); always bound to an Lvalue

// Rvalue:
// - does not have a name
// - temporaries
// - cannot be assigned a value
// - some expressions return Rvalue
// - Rvalue does not persist beyond the expression
// - functions that return by value return Rvalue
// - from C++11: it is possible to create a reference to it ("Rvalue reference"); always bound to an Rvalue
void lvalues_rvalues_demo() {
    std::cout << "lvalues_rvalues_demo::run()" << std::endl;

    // A constant (1) is Rvalue
    1;

    // x is Lvalue
    int x = 1;

    // y is Lvalue
    int y = 2;

    // expression x + y returns Rvalue
    x + y;
    int z = x + y;

    // ++x, x++ return Lvalue
    ++x; // increment x and then return it
    assert(x == 2);

    x++; // return x and then increment it
    assert(x == 3);

    ++x = 10;
    assert(x == 10);

    // error: lvalue required as left operand of assignment
    // add(1, 2) = 1;

    increment(y) = 11;
    assert(y == 11);

    int &&res1 = Square(8);
    const int &res2 = Square(20);
    int res3 = Square(Square(5));
}

// void print(int n) {
//     std::cout << "print(int). n = " << n << std::endl;
// }

void print(int& n) {
    std::cout << "print(int&). n = " << n << std::endl;
}

void print(const int& n) {
    std::cout << "print(const int&). n = " << n << std::endl;
}

// if commented, print(const int&) will be called for temporary values
// if uncommented, this overload will be called for temporary values =>
// this function can be used to test if some value is a temporary one.
void print(const int&& n) {
    std::cout << "print(const int&&). n = " << n << std::endl;
}

// Rvalue reference
// - introduced in C++11 to implement move semantics
// - always created to a temporary value
// - represents a temporary value
// - created with && operator
// - cannot bind to Lvalue

// https://stackoverflow.com/questions/49141435/why-can-temporary-objects-be-bound-to-const-reference
// https://stackoverflow.com/questions/25025538/why-reference-can-not-capture-temporary-while-const-ref-and-rval-ref-can

void rvalue_reference_demo() {
    std::cout << "\nrvalue_reference_demo()\n" << std::endl;

    // A constant is an Rvalue.
    int&& rn1 = 1;
    assert(rn1 == 1);
    assert(rn1 + 1 == 2);
    std::cout << "rn1 = " << rn1 << std::endl;

    // reassignment
    rn1 = 2;
    assert(rn1 == 2);

    const int&& crn1 = 1;
    assert(crn1 == 1);
    // error: assignment of read-only reference ‘crn1’
    // crn1 = 2;

    // Value of expression (1 + 2) is an Rvalue as it's a temporary
    int&& rn2 = 1 + 2;

    // Function returns by value => it's return value is a temporary
    int&& rn3 = add(1, 2);
    const int&& crn3 = add(1, 2);

    int n4 = 1;
    // error: cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
    // int&& rn4 = n4;
    int& rn4 = n4;

    // error: cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’
    // int& rn41 = 1;
    // error: cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’
    // int& rn42 = add(1, 2);

    //  error: cannot bind rvalue reference of type ‘int&&’ to lvalue of type ‘int’
    // int&& rn5 = increment(n4);
    int& rn5 = increment(n4);

    // const Lvalue reference can bind both to Rvalue and Lvalue
    const int& crn6 = increment(n4);
    const int& crn61 = add(1, 2);

    const int& crn7 = 1;

    int n5 = 5;
    print(n5);      // print(int&). n = 5
    print(n5 + 1);  // print(const int&). n = 6

    // very interesting (!)
    print(n5++);    // print(const int&). n = 5
    assert(n5 == 6);

    // very interesting (!)
    print(++n5);    // print(int&). n = 7
    assert(n5 == 7);

    print(16);      // print(const int&). n = 16
    print(16 + 1);  // print(const int&). n = 17

// if print(const int&&) is uncommented, the output will be:
// print(int&). n = 5
// print(const int&&). n = 6
// print(const int&&). n = 5
// print(int&). n = 7
// print(const int&&). n = 16
// print(const int&&). n = 17

}

// Copy Semantics
// - copy of the object is created through copy c-tor
// - copy c-tor copies the state of the object
// - copy c-tor can perform either shallow or deep copy
// - deep copy is required for objets that allocate resources (memory, handles etc...)
// e.g. if class has a pointer member, it manages some memory and this memory content
// also has to be copied
// - in some cases, a copy of a temporary object is created; this copy object has to allocate
// memory and copy the content of the original's memory and this is wasteful as
// we allocate same amount of memory and copy bytes only to see how destruction of temporary
// deallocates original memory...a memory already allocated and which could simply be transfered
// to a new object..
// Example: function returns custom type object by value; in this case a temp object is created

// Move Semantics
// - introduced for faster way of copying objects
// - if we look the example above, Move means that the ownership of the resources will be moved
// to a new object
// - if temp object contains pointer to a memory, during move construction this address will be
// copied to a new object's pointer and old pointer will be set to null so when temp object
// gets destroyed, no memory deallocation takes place (delete nullptr does nothing)
// - this way a new object "steals" resources from the original (temp) object.
// - state of the original/other object is *moved* to the new/copy object.
// - as no memory allocation and data copying takes place, this is faster way to copy from temp
// - Move Semantics should be used if some expression returns a temporary which has to be copied
// to some other object. To detect this temporary we need to implement "move constructor" which
// will be used automatically if original object is a temporary.

class Integer {
    int* pVal_ {};
public:
    Integer() {
        std::cout << "Integer::Integer()" << std::endl;
        pVal_ = new int(0);
    }

    Integer(int n) {
        std::cout << "Integer::Integer(int). n = " << n << std::endl;
        pVal_ = new int(n);
    }

    // copy c-tor
    // It allocates memory and copies data into it from original object.
    // If original object is a temporary, this is not efficient as copy should simply steal the ownership of memory
    // from the original/other object. This is done through move constructor - see class Integer2.
    Integer(const Integer& other) {
        std::cout << "Integer::Integer(const Integer&). other.GetValue() = " << other.GetValue() <<  std::endl;
        pVal_ = new int(other.GetValue());
    }

    ~Integer() {
        std::cout << "Integer::~Integer()" << std::endl;
        delete pVal_;
    }

    void SetValue(int n) {
        delete pVal_;
        pVal_ = new int(n);
    }

    int GetValue() const {
        return *pVal_;
    }
};

class Integer2 {
    int* pVal_ {};
public:
    Integer2() {
        std::cout << "Integer2::Integer2()" << std::endl;
        pVal_ = new int(0);
    }

    Integer2(int n) {
        std::cout << "Integer2::Integer2(int). n = " << n << std::endl;
        pVal_ = new int(n);
    }

    // copy c-tor
    // It allocates memory and copies data into it from original object.
    // If original object is a temporary, this is not efficient as copy should simply steal the ownership of memory
    // from the original/other object. This is done through move constructor.
    Integer2(const Integer2& other) {
        std::cout << "Integer2::Integer2(const Integer2&). other.GetValue() = " << other.GetValue() <<  std::endl;
        pVal_ = new int(other.GetValue());
    }

    // Move Constructor
    // It moves the state (and ownership over resources) from other object to this one.
    // It does not take const rvalue reference.
    // https://stackoverflow.com/questions/10770181/should-a-move-constructor-take-a-const-or-non-const-rvalue-reference
    Integer2(Integer2&& other) {
        std::cout << "Integer2::Integer2(Integer2&&)" << std::endl;
        pVal_ = other.pVal_;

        // let know the other object we took the ownership over resource
        other.pVal_ = nullptr;
    }

    // "The Rule of 5"
    // If class has custom implementation of any of these 5 member methods, all of them should be implemented.
    // - destructor
    // - copy constructor
    // - copy assignment operator
    // - move constructor
    // - move assignment operator
    //
    // Compiler will not automatically generate move c-tor and move assignment operator if user implements
    // either copy c-tor or copy assignment operator.
    // Implementation of a copy constructor or assignment or destructor will block implicit generation of move operations.

    ~Integer2() {
        std::cout << "Integer2::~Integer2()" << std::endl;
        delete pVal_;
    }

    void SetValue(int n) {
        delete pVal_;
        pVal_ = new int(n);
    }

    int GetValue() const {
        return *pVal_;
    }
};

Integer add(const Integer& n1, const Integer& n2){
    Integer n;
    n.SetValue(n1.GetValue() + n2.GetValue());
    return n;
    // return Integer(n1.GetValue() + n2.GetValue());
}

Integer2 add2(const Integer2& n1, const Integer2& n2){
    Integer2 n;
    n.SetValue(n1.GetValue() + n2.GetValue());
    return n;
    // return Integer(n1.GetValue() + n2.GetValue());
}

// If copy elision is desibled in compiler options the output should be like this:
//
// Integer::Integer(int). n = 1
// Integer::Integer(int). n = 2
// Integer::Integer()
// Integer::Integer(const Integer&). other.GetValue() = 3
// Integer::~Integer()
// Integer::~Integer()
// Integer::~Integer()
// Integer::~Integer()
//
// Integer2::Integer2(int). n = 1
// Integer2::Integer2(int). n = 2
// Integer2::Integer2()
// Integer2::Integer2(Integer2&&)
// Integer2::~Integer2()
// Integer2::~Integer2()
// Integer2::~Integer2()
// Integer2::~Integer2()
void move_semantics_demo() {
    std::cout << "\nmove_semantics_demo()\n" << std::endl;

    {
        Integer n1(1), n2(2);

        // Disable copy elision in compiler options in order to make copy-ctor being called.
        // GetValue() is called on a temporary object.
        n1.SetValue(add(n1, n2).GetValue());
    }

    {
        Integer2 n21(1), n22(2);

        // Disable copy elision in compiler options in order to make copy-ctor being called.
        // GetValue() is called on a temporary object.
        n21.SetValue(add2(n21, n22).GetValue());
    }
}

void run(){
    std::cout << "\n\n ***** declarations_demo::run() ***** \n\n" << std::endl;
    // auto_demo();
    // const_demo();
    // const_with_ptrs_and_refs_demo();
    // constexpr_demo();
    constexpr_demo_2();
    // factorial_demo();
    // fibonacci_demo();
    // namespace_demo();
    // lvalues_rvalues_demo();
    // rvalue_reference_demo();
    // move_semantics_demo();
}

}
