#include <initialization_demo.hpp>
#include <iostream>
#include <vector>
#include <cassert>

namespace initialization_demo{

// Uniform Initialization
// works at compile time.
// Scalar types are initialized in the same way as vector types - with initializer list.
// It automatically initializes the variable to 0 (or pointers to nullptr), thereby preventing runtime bugs.
//
// https://en.cppreference.com/w/cpp/language/list_initialization
// https://stackoverflow.com/questions/21825933/any-difference-between-copy-list-initialization-and-traditional-copy-initializat
//
void list_initialization_demo() {

    // value-initialization
    // the variable is initialized with the default value of the given type
    int n1{};
    assert(n1 == 0);

    // direct-list-initialization
    int n2{123};
    assert(n2 == 123);

    char ch2 {'w'};
    assert(ch2 == 'w');

    float f3 {1.41f};
    assert(f3 == 1.41f);

    double d1 {1.41};
    assert(d1 == 1.41);

    // copy-list-initialization
    int n3 = {};
    assert(n3 == 0);

    // copy-list-initialization
    int n4 = {0};
    assert(n4 == 0);

    // warning: implicit conversion from 'int' to 'char' changes value from 129 to -127 [-Wconstant-conversion]
    char ch1 = 129;

    // error: constant expression evaluates to 129 which cannot be narrowed to type 'char' [-Wc++11-narrowing]
    // char ch2 = { 129 };

    double d = 12.34;
    float f1 = d;

    // error: non-constant-expression cannot be narrowed from type 'double' to 'float' in initializer list [-Wc++11-narrowing]
    // float f2 = { d };

    // OK
    float f2 = { 12.34 };

    // warning: implicit conversion from 'double' to 'int' changes value from 12.34 to 12 [-Wliteral-conversion]
    int i1 = 12.34;

    int i2 = f2;
    assert(i2 == 12);

    //
    // initializer list detects "Narrowing of data"
    //

    // error: type 'double' cannot be narrowed to 'int' in initializer list [-Wc++11-narrowing]
    // error: narrowing conversion of ‘1.234e+1’ from ‘double’ to ‘int’ inside { } [-Wnarrowing]
    // int i2 = { 12.34 };

    // warning: narrowing conversion of ‘f2’ from ‘float’ to ‘int’ inside { } [-Wnarrowing]
    int i3{f2};

    //  warning: narrowing conversion of ‘i3’ from ‘int’ to ‘float’ inside { } [-Wnarrowing]
    float f4{i3};
    assert(f4 == 12.0f);

    // v1 will be of type: int T[5]
    // This worked even before C++11
    int arr1[] = {1, 2, 3, 4, 5};
    // this prints: arr1 = 0x7ffd47a329b0 (array decays to pointer)
    std::cout << "arr1 = " << arr1 << std::endl;

    // This is possible from C++11
    // That's why using {} intializer list is called Uniform initialization - scalars, arrays and vectors can be initialized with the same construct.
    // Behind the scenes, whenver compiler sees {...} it creates an instance of std::initialzer_list<T>
    std::vector<int> v1 = {1, 2, 3, 4, 5};

    std::cout << "Vector v1: ";
    for (int i : v1) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    int arr2[3];
    // You cannot assign directly to an array after its declaration:
    // error: assigning to an array from an initializer list
    // arr2 = {6, 7, 8};
    // @todo: use std::copy() to assign initializer list elements to array

    //std::initializer_list<int> il1 = {1, 3, 5, 7};
    //std::cout << "il1 = " << il1 << std::endl;

    std::cout << "arr3 elements: " << "\n";
    int arr3[]{1, 2, 3};
    for(auto i : arr3) {
        std::cout << i << "\n";
    }

    int* pInt {};
    assert(pInt == nullptr);
}

void run() {
    std::cout << "initialization_demo::run()" << std::endl;
    list_initialization_demo();
}

}
