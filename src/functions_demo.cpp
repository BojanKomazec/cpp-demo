#include <functions_demo.hpp>
#include <iostream>
#include <cassert>

namespace functions_demo {

int add(int a, int b) {
    std::cout << "add(int, int)" << std::endl;
    return a + b;
}

// Function Overloading: multiple functions can have the same name but must have different
// type and number of arguments.
// This is achieved by Name Mangling where compiler creates unique name for each function and
// that name depends on name and count of args.
// https://en.wikipedia.org/wiki/Name_mangling
//
// If we look in the .map file, name of this function will be mangled, it will be "functions_demo::add(double, double)"
//
double add(double a, double b){
    std::cout << "add(double, double)" << std::endl;
    return a + b;
}

// C++ name mangling can be suppresssed by using extern "C" directive.
// This allows calling C++ functions from C code.
// If we look in the .map file, name of this function will not be mangled, it will be "add"
extern "C" float add(float a, float b) {
    std::cout << "add(float, float)" << std::endl;
    return a + b;
}

// Function Overloading by changing only return type is not possible.
// This is for functions can be called without assigning their value to any variable in which
// case compiler won't know which function to call as it would use only type/number of args to
// determine which function should be called.
//
// error: ambiguating new declaration of ‘int add(double, double)’
// int add(double a, double b){
//     return 1;
// }

void function_overloading_demo() {
    // compiler deducts which overload to call by looking the type and number of arguments passed to function
    add(3, 4);      // add(int, int)
    add(3.0, 4.0);  // add(double, double)
}

void create_employee(const char* name, bool isFullTime = true) {
    std::cout << "create_employee(): isFullTime = " << isFullTime << std::endl;
}

void default_function_arguments_demo() {
    create_employee("Alice");
    create_employee("Bob", false);
}

int square(int x) {
    return x * x;
}

// macro
#define SQUARE(x) x*x

// inline gives compiler a hint (it does not force it) to perform a function expansion:
// it replaces calls to this function with the body of the function; copies of the
// code in fucntion body will be injected wherever this function is called.
inline int square_inlined(int x) {
    return x*x;
}

void inline_function_demo(){
    auto n0 = 3;

    auto n1 = square(n0);
    assert(n1 == 9);

    auto n2 = SQUARE(n0);
    assert(n1 == 9);

    // Macros work on the principle of text substitution which can introduce subtle bugs.
    // This will be turned into 3+1*3+1 which is 7 but we expect 16.
    auto n3 = SQUARE(n0 + 1);
    assert(n3 == 7);
    // app crashes in runtime: Assertion `n3 == 16' failed.
    // assert(n3 == 16);

    auto n4 = square_inlined(n0);
}

void at_exit_handler(){
    std::cout << "This line should be shown at the program's exit." << std::endl;
}

// function pointers are usefule when the function to be invoked is not known in compile time.
// They allow any type of function to be used at runtime.
// This way we can use different implementations and switch between them while the program is running.
void function_pointer_demo() {
    int(*pAddInt)(int, int) = &add;
    auto sum = pAddInt(3, 4);
    assert(sum == 7);

    // it can be called also as dereferenced pointer
    sum = (*pAddInt)(3, 4);
    assert(sum == 7);

    // ampersand is optional in the initializer as
    // nime of the function is its address.
    int(*pAddInt2)(int, int) = add;
    sum = pAddInt2(4, 5);
    assert(sum == 9);

    // standard C function which accepts argument of type: void (*) (void)
    atexit(at_exit_handler);
}

void run() {
    std::cout << "\n\n ***** functions_demo::run() ***** \n\n" << std::endl;
    function_overloading_demo();
    default_function_arguments_demo();
    inline_function_demo();
    function_pointer_demo();
}

}
