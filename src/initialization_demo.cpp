#include <initialization_demo.hpp>
#include <iostream>

void list_initialization_demo() {
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

    // error: type 'double' cannot be narrowed to 'int' in initializer list [-Wc++11-narrowing]
    // int i2 = { 12.34 };
}
void initialization_demo() {
    std::cout << "initialization_demo()" << std::endl;
    list_initialization_demo();
}
