#include <templates_demo.hpp>
#include <iostream>
#include <cassert>
#include <cstring>

namespace templates_demo {

namespace introduction{

int max(int n1, int n2) {
    return n1 > n2 ? n1 : n2;
}

// If we want to create a function with the same functionality applied on another data type, we can create its overload:
float max(float f1, float f2) {
    return f1 > f2 ? f1 : f2;
}


// To cover all (numeric) types we'd need to implement many overloads.
// Templates allow creating functions that can be used on any type.
//
// This is a template function.
// Begins with 'template' keyword followed by template parameter list.
// Keyword 'class' can be used instead of 'typename'.
// Compiler will generate functions for only those types it is invoked with.
// Compiler deduces types from the function call and then generates functions for those types.
// If function template is not invoked, function is not generated at all by the compiler.
// The process by which the function is generated is called instantiation.
// Template functions are generated in compile time, no runtime costs are involved.
//
template <typename T>
T max(T arg1, T arg2) {
    return arg1 > arg2 ? arg1 : arg2;
}

template <typename T>
T add(const T& t1, const T& t2) {
    return t1 + t2;
}

template <typename T>
T arrSum(T* arr, size_t size) {
    T sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += arr[i];
    }

    return sum;
}

template <typename T>
T arrMax(T* arr, size_t size) {
    T max = arr[0];
    for (size_t i = 0; i < size; ++i) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    return max;
}

// return min and max element in array
template<typename T>
std::pair<T,T> arrMinMax(T *arr, size_t size){
    std::pair<T, T> ret{arr[0], arr[0]};
    for (size_t i = 0; i < size; ++i) {
        if (arr[i] < ret.first) {
            ret.first = arr[i];
        }
        if (arr[i] > ret.second) {
            ret.second = arr[i];
        }
    }
    return ret;
}

void demo() {
    auto maxValueInt1 = max(1, 2);
    std::cout << "maxValueInt1 = " << maxValueInt1 << std::endl;
    auto maxValueFloat = max(1.1f, 2.2f);
    std::cout << "maxValueFloat = " << maxValueFloat << std::endl;

    maxValueFloat = max(1.1f, 2.2f);
    std::cout << "maxValueFloat = " << maxValueFloat << std::endl;

    auto sumFloat = add(3.14f, 1.01f);
    std::cout << "sumFloat = " << sumFloat << std::endl;

    float arrFloat[] = { 3.14f, 1.01f };
    sumFloat = arrSum(arrFloat, 2);
    std::cout << "sumFloat = " << sumFloat << std::endl;

    float arrFloat2[] = { 3.14f, 1.01f, 5.23f };
    maxValueFloat = arrMax(arrFloat2, 3);
    std::cout << "maxValueFloat = " << maxValueFloat << std::endl;

    auto pair = arrMinMax(arrFloat2, 3);
    std::cout << "pair.first = " << pair.first << ", pair.second = " << pair.second << std::endl;
}

} // namespace

//
// Template Argument Deduction
//
// - performed in compile time
// - process of deducing the types
// - each function argument is examined
// - the corresponding type argument is deduced from the argument
// - type conversions are not performed
// - once the type name is dedued, in subsequent type arguments it should be deduced as the same type
// - after deduction, the template is instantiated
// - sometimes we need to override deduction by specifying desired types in template argument list: max<int>(1, 2)
//
// Template Instantiation
//
// - A template function or class only acts as a blueprint
// - The compiler generates code from the blueprint at compile time
// - This process is called Template Instantiation
// - Occurs implicitly when:
//      - a function template is invoked
//      - taking address of a function template
//      - using explicit instantiation
//      - creating explicit specialization
// - Full definition of template should be available => it must be in header file
namespace template_arg_deduction {

template <typename T>
T max(T arg1, T arg2) {
    std::cout << typeid(T).name() << std::endl;
    return arg1 > arg2 ? arg1 : arg2;
}

// Explicit instantiation of function template causes template to create max function for char type.
// This max function is never called (but is still created/instantiated).
template char max(char ch1, char ch2);

void demo() {
    //
    // Examples where both arguments are of the same type.
    //

    max(3, 5); // i
    max(3.14, 1.03); // d

    //
    // Examples where one arg is of one type and another arg is of another type.
    //

    // error: no matching function for call to ‘max(int, float)’
    // note: candidate: ‘template<class T> T templates_demo::template_arg_deduction::max(T, T)’
    // note:   template argument deduction/substitution failed:
    // note:   deduced conflicting types for parameter ‘T’ (‘int’ and ‘float’)
    // max(3, 5.14f);

    // solution #1: type cast (so both args have the same type)
    max(static_cast<float>(3),  5.14f);

    // solution #2: override type deduction and explicitly specify T type:
    max<float>(3, 5.14f);

    // Instantiation takes place when we take the address of template function.
    // Function max is not called here.
    int (*pfn)(int, int) = max;
}

} // namespace template_arg_deduction

//
// Explicit Specialization
//
// - Template is specialized for a particular type
// - Why?
//      - Provides correct semantics for some data types
//      - Algorithm can be implemented more optimally for a specific type
//        (We can write separate implementation of the function for a specific type)
// - Explictily specialized functions must be defined in .cpp files as they are already
//   instantiated
// - Primary template definition should occur before specialization
namespace explicit_specialization{

template <typename T>
T max(T arg1, T arg2) {
    std::cout << typeid(T).name() << std::endl;
    return arg1 > arg2 ? arg1 : arg2;
}

namespace solution {

template <typename T>
T max(T arg1, T arg2) {
    std::cout << typeid(T).name() << std::endl;
    return arg1 > arg2 ? arg1 : arg2;
}

// Explicit Specialization of a template function.
// We can omit type afger function name but it's more verbose and easier to understand if we specify type explicitly there.
// const char* max(const char* arg1, const char* arg2) {
template<>
const char* max<const char*>(const char* arg1, const char* arg2) {
    std::cout << "max<const char*>" << std::endl;
    return strcmp(arg1, arg2) > 0 ? arg1 : arg2;
}

}

template char max(char ch1, char ch2);

void demo(){
    const char* a{"A"};
    const char* c{"C"};
    const char* b{"B"};

    // We want to use max to compare strings
    auto s1 = max(a, b); // PKc
    auto s2 = max(c, b); // PKc

    // ...but max compares addresses:
    std::cout << "s1 = " << s1 << std::endl; // B - OK.
    std::cout << "s2 = " << s2 << std::endl; // B - WRONG!

    // https://stackoverflow.com/questions/17813423/cout-with-char-argument-prints-string-not-pointer-value
    std::cout << "a = " << static_cast<const void*>(a) << std::endl;
    std::cout << "b = " << static_cast<const void*>(b) << std::endl;
    std::cout << "c = " << static_cast<const void*>(c) << std::endl;
    // a = 0x556d2021babc // min
    // b = 0x556d2021bac0
    // c = 0x556d2021babe // max

    // How can we make max() compare values at the addresses and not addresses themselves?
    // We need to use explicit specialization of a template function.

    auto s11 = solution::max(a, b);
    auto s21 = solution::max(c, b);

    // ...but max compares addresses:
    std::cout << "s11 = " << s11 << std::endl; // B - OK.
    std::cout << "s21 = " << s21 << std::endl; // C - OK.

}
}

// Non-type template arguments
// - appear in template argument list
// - they are always constant expressions
// - are common in template classes but are used with arrays in template functions
// Non-type template argument is:
// - expression that is computed at compile time within a template argument list
// - must be constant expression (address, reference, integral, nullptr, enum)
// - if it's an address, it should be a static address
// - part of the template type
// - used by std::begin() and std::end()
namespace non_type_template_arguments {

template<int size>
void print_size(){

    // error: increment of read-only location ‘size’
    // error: lvalue required as increment operand
    // size++;

    // size is const so can be used to specify size of an array:
    char buffer[size];

    std::cout << "size = " << size << std::endl;
}

template<typename T>
T sum(T* arr, size_t size) {
    T sum{};
    for (auto i = 0; i < size; ++i){
        sum += arr[i];
    }

    return sum;
}

// we can use reference to array and non-type template args we can pass an array to a function without specifying its size.
// This is used by standard library to implement global functions std::begin() and std::end() for arrays.
template<typename T, int size>
T sum2(T (&arr)[size]) {
    T sum{};
    for (auto i = 0; i < size; ++i){
        sum += arr[i];
    }

    return sum;
}

void demo(){
    print_size<123>();

    int n = 456;

    // A variable with non-static storage duration (basically one that can't be evaluated in compile time)
    // cannot be used as a non-type argument.
    //
    // error: no matching function for call to ‘print_size<n>()
    // note: candidate: ‘template<int size> void templates_demo::non_type_template_argument
    // note:   template argument deduction/substitution failed:
    // error: the value of ‘n’ is not usable in a constant expression
    // note: ‘int n’ is not const
    // note: in template argument for type ‘int’
    // print_size<n>();

    // sizeof() is calculated in compile time - it's constant expression so therefoere can be used as a non-type argument.
    print_size<sizeof(n)>();

    const int n2 = 456;
    print_size<n2>();

    int arrInt[]{1, 2, 3};
    auto sumVal1 = sum<int>(arrInt, 3);
    std::cout << "sumVal1 = " << sumVal1 << std::endl;

    // reference to array
    int (&refArrInt)[3] = arrInt;

    // error: invalid initialization of reference of type ‘int (&)[4]’ from expression of type ‘int [3]’
    // int (&refArrInt2)[4] = arrInt;

    // compiler knows how to deduct T and size in type T(&)[size] from int[3]:
    auto sumVal2 = sum2(arrInt);
    std::cout << "sumVal2 = " << sumVal2 << std::endl;

    auto it = std::begin(arrInt);
}
}

namespace misc {

template<typename Function>
struct Defer {
    Function f_;
    // todo: why is https://twitter.com/voidtarget/status/1250338120932843521 using && here?
    Defer(Function f): f_(f){}
    ~Defer(){
        f_();
    }
};

void foo() {
    std::cout << "foo()" << std::endl;
}

void defer_test(){
    std::cout << "defer_test()" << std::endl;
    {
        std::cout << "Before instantiating Defer" << std::endl;
        // Defer<decltype(foo)> d;
        std::cout << "After instantiating Defer" << std::endl;
    }
    std::cout << "~defer_test()" << std::endl;
}

}

void run() {
    std::cout << "templates_demo::run()" << std::endl;
    // introduction::demo();
    // template_arg_deduction::demo();
    // explicit_specialization::demo();
    // misc::defer_test();
    non_type_template_arguments::demo();
}

}
