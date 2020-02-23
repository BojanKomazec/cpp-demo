#include <type_conversions_demo.hpp>
#include <iostream>
#include <cassert>

namespace type_conversions_demo {

void basic_types_casting_demo() {
    std::cout << "basic_types_casting_demo()" << std::endl;

    int n1 = 7, n2 = 2;
    float f1 = n1;
    assert(f1 == 7.0);

    // You can implicitly convert between numerical types, even when that loses precision.
    // int/int = int => decimal part is lost
    f1  = n1/n2;
    assert(f1 == 3.0);

    f1 = 7.0f/n2;
    assert(f1 == 3.5f);

    // C-style cast (discouraged in C++)
    f1 = (float)n1/n2;
    assert(f1 == 3.5f);

    // C-style cast is discouraged in C++ as compiler does not check if one type can actually be converted to another one.
    // We tell to compiler "I know what I'm doing, you shut up". But we developers can make mistakes and force wrong conversions.
    // Some compielers only issue warnings like:
    //  warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
    auto p = (char*)n1;

    // g++ compiler is not issuing a warning here
    // @todo find out why
    p = (char*)&n1;

    //
    // static_cast<T>
    //
    // - checks whether cast is valid or not
    // - issues compiler errors to avoid potentially lossy conversions and conversions of incompatible types
    f1 = static_cast<float>(n1)/n2;

    // compiler now issues an error:
    // error: invalid static_cast from type ‘int’ to type ‘char*’
    // p = static_cast<char*>(n1);

    // error: invalid static_cast from type ‘int*’ to type ‘char*’
    // p = static_cast<char*>(&n1);

    //
    // reinterpret_cast<T>
    //
    // - allows type casting even if types are not related
    // - for special circumstances, converting between pointers or references and completely unrelated types
    // - it won't do numeric conversions (issues an error)
    // - keeps qualifiers (e.g. const) in contrast to C-style cast
    //   (issues an error if cast would trigger loosing qualifiers)
    // - should almost never be used, unless you want to cast one type into a fundamentally different type
    // - it is machine dependent so safely using it requires complete understanding of the types
    //   as well as how the compiler implements the cast

    // no compiler warnings
    p = reinterpret_cast<char*>(&n1);

    // error: invalid cast from type ‘int’ to type ‘float’
    // f1 = reinterpret_cast<float>(n1);

    const int n3 = 3;
    // error: invalid conversion from ‘const int*’ to ‘int*’ [-fpermissive]
    // int* pn = &n3;

    // C-style cast issues warning only:
    // warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
    int* pn = (int*)&n3;

    // reinterpret_cast issues an error about loosing qualifiers.
    // error: reinterpret_cast from type ‘const int*’ to type ‘int*’ casts away qualifiers
    // int* pn2 = reinterpret_cast<int*>(&n3);

    //
    // const_cast<T>
    //
    // - can't change types, only const or volatile qualifiers
    int* pn2 = const_cast<int*>(&n3);
}

class Integer {
    int* pVal_;
public:
    Integer() {
        std::cout << "Integer::Integer()" << std::endl;
        pVal_ = new int(0);
    }
    // Constructor that enables conversion of primitive type int to user-defined type Integer
    // Can be called explicitly or implicitly.
    Integer(int n) {
        std::cout << "Integer::Integer(int). n = " << n << std::endl;
        pVal_ = new int(n);
    }

    Integer(const Integer& other) {
        std::cout << "Integer::Integer(const Integer&). other.GetValue() = " << other.GetValue() << std::endl;
        pVal_ = new int(*other.pVal_);
    }

    Integer& operator=(const Integer& other){
        std::cout << "Integer::operator=(). other.GetValue() = " << other.GetValue() << std::endl;
        if (this != &other) {
            delete pVal_;
            pVal_ = new int(*other.pVal_);
        }
    }

    ~Integer() {
        std::cout << "Integer::~Integer()" << std::endl;
        delete pVal_;
    }

    int GetValue() const {
        return *pVal_;
    }
};

class Integer2 {
    int* pVal_;
public:
    Integer2() {
        std::cout << "Integer2::Integer2()" << std::endl;
        pVal_ = new int(0);
    }
    // explicit prevent implicit calls of parameterized c-tor
    // Allows only explicit calls.
    // Temporary Integer2 object cannot be created from integer (1).
    // Integer2 n = 1; // won't compile
    explicit Integer2(int n) {
        std::cout << "Integer2::Integer2(int). n = " << n << std::endl;
        pVal_ = new int(n);
    }

    Integer2(const Integer2& other) {
        std::cout << "Integer2::Integer2(const Integer2&). other.GetValue() = " << other.GetValue() << std::endl;
        pVal_ = new int(*other.pVal_);
    }

    Integer2& operator=(const Integer2& other){
        std::cout << "Integer2::operator=(). other.GetValue() = " << other.GetValue() << std::endl;
        if (this != &other) {
            delete pVal_;
            pVal_ = new int(*other.pVal_);
        }
    }

    ~Integer2() {
        std::cout << "Integer2::~Integer2()" << std::endl;
        delete pVal_;
    }

    int GetValue() const {
        return *pVal_;
    }

    // Type Conversion Operator
    operator int() {
        return *pVal_;
    }
};

class Integer3 {
    int* pVal_;
public:
    Integer3() {
        std::cout << "Integer3::Integer3()" << std::endl;
        pVal_ = new int(0);
    }
    // explicit prevent implicit calls of parameterized c-tor
    // Allows only explicit calls.
    explicit Integer3(int n) {
        std::cout << "Integer3::Integer3(int). n = " << n << std::endl;
        pVal_ = new int(n);
    }

    Integer3(const Integer3& other) {
        std::cout << "Integer3::Integer3(const Integer3&). other.GetValue() = " << other.GetValue() << std::endl;
        pVal_ = new int(*other.pVal_);
    }

    Integer3& operator=(const Integer3& other){
        std::cout << "Integer3::operator=(). other.GetValue() = " << other.GetValue() << std::endl;
        if (this != &other) {
            delete pVal_;
            pVal_ = new int(*other.pVal_);
        }
    }

    ~Integer3() {
        std::cout << "Integer3::~Integer3()" << std::endl;
        delete pVal_;
    }

    int GetValue() const {
        return *pVal_;
    }

    // C++11: Type Conversion Operator can be declared as explicit
    // in order to prevent its implicit calls. This means that using static_cast<int> is necessary.
    explicit operator int() {
        return *pVal_;
    }
};

void PassIntegerByValue(Integer n) {
    auto val = n.GetValue();
    std::cout << "PassIntegerByValue(). Value = " << val << std::endl;
}

void PassIntegerByConstRef(const Integer& n) {
    auto val = n.GetValue();
    std::cout << "PassIntegerByConstRef(). Value = " << val << std::endl;
}

void primitive_to_user_type_conversion_demo(){
    std::cout << "primitive_to_user_type_conversion_demo()" << std::endl;

    // Integer::Integer(int) called implicitly
    Integer n1 {1};
    std::cout << "n1 value = " << n1.GetValue() << std::endl;

    // Integer::Integer(int) called explicitly
    Integer n2(2);
    std::cout << "n2 value = " << n2.GetValue() << std::endl;

    // Integer::Integer(int) called implicitly
    // If there is no user-defined copy constructor, this is the output:
    //      Integer::Integer(int). n = 3
    //      Integer::~Integer()
    //      n3 value = 0
    // Integer is created from 3 and then default copy constructor is called which performs shallow copy.
    // That's why we should follow The Rule of 3/5:
    //
    //      Integer::Integer(int). n = 3
    //      Integer::Integer(const Integer&). other.GetValue() = 3
    //      Integer::~Integer()
    //      n3 value = 3
    //
    Integer n3 = 3; // intialization = creation and value assignment
    std::cout << "n3 value = " << n3.GetValue() << std::endl;

    // Integer is created from 4. Then copy assignment operator is called.
    //
    //      Integer::Integer()           // n4 creation
    //      Integer::Integer(int). n = 4 // temp Integer creation
    //      Integer::operator=(). other.GetValue() = 4
    //      Integer::~Integer()          // temp Integer destruction
    //      n4 value = 4
    //
    Integer n4;
    n4 = 4; // assignment (not initialization as n4 has already been created)
    std::cout << "n4 value = " << n4.GetValue() << std::endl;

    // This example shows that compiler tries to find a constructor which accepts builtin type.
    // If it doesn't then it issues an error.
    // error: invalid conversion from ‘const char*’ to ‘int’ [-fpermissive]
    // note: initializing argument 1 of ‘type_conversions_demo::Integer::Integer(int)’
    // Integer ns = "string";

    // Integer::Integer(int) called implicitly
    // If there is no user-defined copy assignment operator, this is the output:
    //      Integer::Integer(int). n = 1
    //      PassIntegerByValue(). Value = 1
    //      Integer::~Integer()
    //      Integer::~Integer()
    //
    // We can see that Integer is created from 1.
    PassIntegerByValue(1);

    // Integer::Integer(int) called implicitly
    // If there is no user-defined copy assignment operator, this is the output:
    //      Integer::Integer(int). n = 2
    //      PassIntegerByConstRef(). Value = 2
    //      Integer::~Integer()
    //      Integer::~Integer()
    PassIntegerByConstRef(2);

    // Integer2::Integer2(int) is declared as 'explicit'
    // error: conversion from ‘int’ to non-scalar type ‘type_conversions_demo::Integer2’ requested
    // Temporary Integer2 object cannot be created from integer (5).
    // Integer2 n5 = 5;
    Integer2 n5(5);
}

void user_to_primitive_type_conversion_demo(){
    std::cout << "user_to_primitive_type_conversion_demo()" << std::endl;

    Integer n1(1);

    // error: cannot convert ‘type_conversions_demo::Integer’ to ‘int’ in initialization
    // int n = n1;

    Integer2 n2;
    // Type Conversion Operator is called implicitly
    int n = n2;

    Integer3 n3;
    // Type Conversion Operator is declared 'explicit' in Integer3.
    // error: cannot convert ‘type_conversions_demo::Integer3’ to ‘int’ in assignment
    // n = n3;
    // Cast has to be done explicitly:
    n = static_cast<int>(n3);
}

void run() {
    std::cout << "type_conversions_demo::run()" << std::endl;
    basic_types_casting_demo();
    primitive_to_user_type_conversion_demo();
    user_to_primitive_type_conversion_demo();
}

}
