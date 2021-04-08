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
// We can omit type after function name but it's more verbose and easier to understand if we specify type explicitly there.
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

namespace perfect_forwarding {
class Integer{
    int* pInt_;
public:
    // Default constructor
    Integer(){
        std::cout << "Integer()" << std::endl;
    }

    // Parameterized constructor
    Integer(int n):pInt_(new int{n}){
        std::cout << "Integer(int n)" << std::endl;
    }

    // Copy constructor
    Integer(const Integer& other):pInt_(new int{*other.pInt_}){
        std::cout << "Integer(const Integer&)" << std::endl;
    }

    // Move constructor
    Integer(Integer&& other):pInt_(other.pInt_){
        std::cout << "Integer(Integer&&)" << std::endl;
        other.pInt_ = nullptr;
    }

    // Destructor
    ~Integer(){
        std::cout << "~Integer()" << std::endl;
        if (pInt_) {
            delete pInt_;
            pInt_ = nullptr;
        }
    }

    int GetValue() const {
        return *pInt_;
    }

    void SetValue(int n){
        *pInt_ = n;
    }
};

// Employee implementation without move constructor:
class Employee1{
    std::string name_;
    Integer id_;
public:
    Employee1(const std::string& name, const Integer& id):
        name_(name), id_(id){}
};

// Employee implementation with move constructor:
class Employee2{
    std::string name_;
    Integer id_;
public:
    Employee2(const std::string& name, const Integer& id):
        name_(name), id_(id){
        std::cout << "Employee2(const std::string& name, const Integer& id)" << std::endl;
    }

    Employee2(std::string&& name, Integer&& id):
        name_(name), id_(id){
        std::cout << "Employee2(std::string&& name, Integer&& id)" << std::endl;
    }
};

// Employee implementation with move constructor which calles std::move on Integer parameter:
class Employee3{
    std::string name_;
    Integer id_;
public:
    Employee3(const std::string& name, const Integer& id):
        name_(name), id_(id){
        std::cout << "Employee3(const std::string& name, const Integer& id)" << std::endl;
    }

    Employee3(std::string&& name, Integer&& id):
        name_(name), id_(std::move(id)){
        std::cout << "Employee3(std::string&& name, Integer&& id)" << std::endl;
    }
};

// Constructor is a member template function
//
// Reference Collapsing Rule for Function templates that accept rvalue references as arguments:
// (Parameter is variable in the declaration of function.
//  Argument is the actual value of this variable that gets passed to function.)
//
// - If lvalues are passed as arguments to this function template then the type of T1 and T2 will become lvalue references.
//   name and id will remain lvalue references.
//   This means we'd have: (T1& name, T2& id).
// - If rvalues are passed as arguments into this function then args will remain rvalue references.
//   name and id will become rvalue references.
//   We'd have as it is: (T1&& name, T2&& id).
//
// This rule does not apply if arguments were lvalue references as arguments e.g. Employee4(T1& name, T2& id)
//
class Employee4{
    std::string name_;
    Integer id_;
public:
    template<typename T1, typename T2>
    Employee4(T1&& name, T2&& id):
        name_(name), id_(id){
        std::cout << "Employee4(T1&& name, T2&& id)" << std::endl;
    }
};

// Perfect forwarding
// If name and id are rvalue references, they will passed further as rvalue references.
// Reference to named temp object will not be lvalue reference but rvalue reference.
// If name and id are lvalue references, they will passed further as value references.
// Information about the reference type is preserved when the argument is forwarded to the next function call.
class Employee5{
    std::string name_;
    Integer id_;
public:
    template<typename T1, typename T2>
    Employee5(T1&& name, T2&& id):
        name_(std::forward<T1>(name)),
        id_(std::forward<T2>(id)){
        std::cout << "Employee5(T1&& name, T2&& id)" << std::endl;
    }
};

// Factory method without perfect forwarding:
template<typename T1, typename T2>
Employee5* CreateEmployee51(T1&& name, T2&& id){
    return new Employee5(name, id);
}

// Factory method with perfect forwarding:
template<typename T1, typename T2>
Employee5* CreateEmployee52(T1&& name, T2&& id){
    return new Employee5(std::forward<T1>(name), std::forward<T2>(id));
}

void show_problem(){
    {
        Employee1 employee{"Bojan", 123};

        // (*) Although we are passing temp Integer object, when assigning value to id_ a copy c-tor is called, another new object is created:
        // Integer(int n) // temp object is created
        // Integer(const Integer&) // *
        // ~Integer() // temp Integer object destroyed
        // ~Integer() // id_ Integer object destroyed
        // What if we call move constructor?
    }

    {
        Employee2 employee{"Bojan", 123};

        // Employee2's move c-tor is called as temp objects are passed.
        // https://stackoverflow.com/a/39348385/404421
        // With move semantics the compiler will inspect whether the argument is an rvalue or not.
        // If class provides a (move) constructor overload and c-tor param is an rvalue reference, then move c-tor will be preferred.
        // Compiler knows thate these temp objects will not be used any more, therefore it doesn't make sense to copy them since they will
        // just be destructed at once anyway. So the compiler decides to call the move-constructor instead.

        // Output:
        // Integer(int n)
        // Integer(const Integer&)  // copy c-tor is (still) invoked when id_ is initialized :-(
        // Employee2(std::string&& name, Integer&& id)
        // ~Integer()
        // ~Integer()

        // Integer's copy c-tor is still invoked because rvalue reference variable has a name (name of the argument passed, "id" in "Integer&& id").
        // This "id" variable is not a temporary, it is a lvalue => copy c-tor is called.
    }

    {
        Employee3 employee3{"Bojan", 123};

        // Output:
        // Integer(int n)
        // Integer(Integer&&) <-- move c-tor is called, finally!
        // Employee3(std::string&& name, Integer&& id)
        // ~Integer()
        // ~Integer()
    }

    {
        std::string name{"Ada"};
        Employee3 employee3{name, 123};

        // Output:
        // Integer(int n)
        // Integer(const Integer&) // copy c-tor called again :-(
        // Employee3(const std::string& name, const Integer& id)
        // ~Integer()
        // ~Integer()

        // Solution: implement Employee3(const std::string& name, Integer&& id)
    }

    {
        Integer n{123};
        Employee3 employee3{"Bob", n};

        // Output:
        // Integer(int n)
        // Integer(const Integer&)
        // Employee3(const std::string& name, const Integer& id)
        // ~Integer()
        // ~Integer()

        // Solution: implement Employee3(std::string&& name, const Integer& id)
    }

    // Problem:
    // It would be tedious implementing all these variations of constructors.
    // Solution:
    // Implement a single constructor but as a template function.

    {
        Employee4 employee4{"Bojan", 123};

        // Output:
        // Integer(int n) // T2 deducted as int; id_ is initialized via parameterized c-tor
        // Employee4(T1&& name, T2&& id)
        // ~Integer()
    }

    {
        Employee4 employee4{"Bojan", Integer{123}};

        // Output:
        // 2 instances of Integer are created again :-(
        // Integer(int n) // unnamed temp variable is created (function parameter is always created before it's passed to function)
        // Integer(const Integer&) // temp variable rvalue reference (Integer&&) now has name assigned via argument name - "id" so copy c-tor is invoked;
        //                         // id basically becomes lvalue reference: Integer&
        // Employee4(T1&& name, T2&& id) // T2 is deduced as Integer
        // ~Integer()
        // ~Integer()

        // So, how can we pass named rvalue reference (of type Integer&&) to id_{id} as rvalue reference?
        // We need to use perfect forwarding.
    }
}

// In all these examples the goal is to prevent unnecessary creation of objects <==> reusing existing (usually temp) objects via move c-tors.
void show_solution() {
    {
        // Both args are temp objects.
        // We want employee to contain exactly date contained in these objects, without creating new objects and copying data.
        // Temp Integer will be passed as rvalue ref (parameter id will be of type Integer&&)
        Employee5 employee{"Bojan", Integer{123}};
        // Output:
        // Integer(int n) // create temporary - function argument
        // Integer(Integer&&) // Before Employee5 move c-tor is called, call move c-tor for class member id_
        //                    // employee takes ownership over temp's data - exactly what we want!
        // Employee5(T1&& name, T2&& id) // body of the move c-tor for class
        // ~Integer() // destroy temp object
        // ~Integer() // destroy member id_
    }

    {
        // name is temp (passed as rvalue ref), id is non-temp (passed as lvalue ref).
        // In this case we want to make copy of n's value and use it to initialize a newly created id_ object.
        // We don't want employee object to take ownership over n's data as n might be used after Employee5 c-tor call.
        // n will be passed as lvalue ref (parameter id will be of type Integer&)
        Integer n{123};
        Employee5 employee{std::string{"Bojan"}, n};
        // Output:
        // Integer(int n)
        // Integer(const Integer&) // copy c-tor is called - exactly what we want!
        // Employee5(T1&& name, T2&& id)
        // ~Integer()
        // ~Integer()
    }
}

// As both args are temp objects, we want to see move c-tors called here so their data is reaused by employee object taking ownership over it.
// But that doesn't happen as both args are named temp objects inside factory function so they are forwarded as lvalue references to next function call
// which is Employee5 c-tor.
void show_factory_method_problem() {
    Employee5* pEmployee5 = CreateEmployee51(std::string{"Bojan"}, Integer{123});
    // Output:
    // Integer(int n)
    // Integer(const Integer&) // copy c-tor is invoked (a new object is created and temp's data is copied to it) - we don't want this!
    // Employee5(T1&& name, T2&& id)
    // ~Integer()
}

void show_factory_method_solution() {
    Employee5* pEmployee5 = CreateEmployee52(std::string{"Bojan"}, Integer{123});
    // Output:
    // Integer(int n)
    // Integer(Integer&&) // move c-tor is calle - exactly what we want!
    // Employee5(T1&& name, T2&& id)
    // ~Integer()
}

void demo(){
    // show_problem();
    // show_solution();
    // show_factory_method_problem();
    show_factory_method_solution();
}
} // namespace

// Variadic Templates
// - functions and classes that can accept arbitrary number of arguments
// - in C printf() can accept any number of arguments; internally, it's implemented through macros =>
//      - it's not type safe
//      - cannot accept references as arguments
// https://eli.thegreenplace.net/2014/variadic-templates-in-c/git
namespace variadic_templates{

// Use C++11 initializer list to implement C-style printf()
template<typename T>
void Print(std::initializer_list<T> il) {
    std::cout << "Print()" << std::endl;
    // for (auto el : il) {
    for (const auto& el : il) {
        std::cout << el << std::endl;
    }
}

// By using variadic templates we can create a function which can accept any number of arguments of any type.
// Three dots (...) are known as ellipsis and they specify that this template method is a variadic template one.
// (they are also used in catch block where we want to specify that it can catch any exception).
// We need also to specify a name that represents the variable number of type names: Params.
// Params does not represent a type name. It is an alias to the list of type names.
// It is a name of the set of type names.
// This is known as "template parameter pack"
template<typename...Params>
void Print2(Params... args){
}

// How can we access individual arguments?
// Individual argumets can't be accessed directly.
// To access individual arguments we need to use recursion: this funciton has to call itself and pass 'args' to it.
// Technically, this is not recursion, because a different function is called. The compiler ends up generating a different function
// for every used length of the parameter pack. It's useful to reason about it recursively, though.
// In each recursive call number of arguments in 'args' will be reduced by 1.
// Just like in other recursion, we need to define the stop condition (base case, which stops recursion).
// If we don't do it we'll get infinite recurson (Segmentation fault )...
template<typename...Params>
void Print3(Params... args){
    // Print3(args);
    //  error: parameter packs not expanded with ‘...’:

    // We need to expand parameter pack with ellipsis when passing it to another function call:
    Print3(args...);
}

// That stop condition is implemented via another Print function which does not accept variadic templates and which is passed to
// variadic template function.
// That function is called a Base Case function and we'll pass it via t argument:
// In each recursive call number of arguments in 'args' will be reduced by 1.
// In the final call 0 arguments will be passed.
template<typename T, typename...Params>
void Print4(T arg0, Params... args){
    Print4(args...);
}

//
// Final version: variadic template function with base case function
//

// Base Case function must be declared BEFORE its variadic template function!
void Print5() {
    std::cout << std::endl;
}

// Good but not perfect. This will print one extra comma after the last argument.
// To avoid printing it, we need to know if args is empty.
// We need to know the number of arguments in the function parameter pack.
template<typename T, typename...Params>
void Print5(T arg0, Params... args){
    std::cout << arg0 << ", ";
    Print5(args...);
}

// To get the number of arguments in the template parameter pack we need to use variadic sizeof operator.
void Print6() {
    std::cout << std::endl;
}

template<typename T, typename...Params>
void Print6(T arg0, Params... args){
    std::cout << "sizeof...(Params) = " << sizeof...(Params) << std::endl;
    std::cout << "sizeof...(args) = " << sizeof...(args) << std::endl;

    std::cout << arg0 << ", ";

    Print6(args...);
}


void Print7() {
    std::cout << std::endl;
}

template<typename T, typename...Params>
void Print7(T arg0, Params... args){
    std::cout << arg0;

    if (sizeof...(args) != 0) {
        std::cout << ", ";
    }

    Print7(args...);
}

std::ostream& operator<< (std::ostream& os, const perfect_forwarding::Integer& n){
    os << n.GetValue();
    return os;
}

void Print8() {
    std::cout << std::endl;
}

template<typename T, typename...Params>
void Print8(const T& arg0, const Params&... args){
    std::cout << arg0;

    if (sizeof...(args) != 0) {
        std::cout << ", ";
    }

    Print8(args...);
}

void Print9() {
    std::cout << std::endl;
}

// Reference collapsing rule applies here:
// If lvalue is passed to Print9's arg0, arg0 becomes lvalue reference.
// If rvalue is passed to Print9's arg0, arg0 becomes rvalue reference.
// Same applies for args.
template<typename T, typename...Params>
void Print9(T&& arg0, Params&&... args){
    std::cout << arg0;

    if (sizeof...(args) != 0) {
        std::cout << ", ";
    }

    Print9(args...);
}

void Print10() {
    std::cout << std::endl;
}

template<typename T, typename...Params>
void Print10(T&& arg0, Params&&... args){
    std::cout << arg0;

    if (sizeof...(args) != 0) {
        std::cout << ", ";
    }

    Print10(std::forward<Params>(args)...);
}

void show_problem(){
    Print({1, 2, 3});

    // Problem with initializer list is that all elements have to be of the same type oterwise template type deduction would fail:
    // error: no matching function for call to ‘Print(<brace-enclosed initializer list>)’
    // note:   template argument deduction/substitution failed:
    // note:   deduced conflicting types for parameter ‘_Tp’ (‘int’ and ‘double’)
    // Print({1, 2, 3.14});

    // Solution: using variadic templates.

    // arguments passed here are expanded to 'args'.
    Print2(1, "test", 3.14);

    // Print3(1, "test", 3.14);
    // Segmentation fault (core dumped)

    // Print4(1, "test", 3.14);
    // Output:
    // error: no matching function for call to ‘Print4()’
    // note: candidate: ‘template<class T, class ... Params> void templates_demo::variadic_templates::Print4(T, Params ...)’
    // note:   template argument deduction/substitution failed
    // note:   candidate expects at least 1 argument, 0 provided
    //
    // Explanation of these recursive calls:
    //
    // Print4(1, "test", 3.14); => arg0 = 1, args = {"test", 3.14}
    // Print4("test", 3.14); => arg0 = "test", args = {3.14}
    // Print4(3.14) =>  arg0 = 3.14, args = {} <-- args is now empty list!
    // Print4() => error no matching function for call to ‘Print4()’; as we haven't provided such function.
}

void show_solution(){
    Print5(1, "test", 3.14);
    // Output:
    // 1, test, 3.14,
    // Good but not perfect. There is one extra comma after the last argument.
    // To avoid printing it, we need to know if args is empty.

    Print6(1, "test", 3.14);


    Print7(1, "test", 3.14);
    // Output:
    // 1, test, 3.14 <-- this is what we wanted! :-)

    // But, there is still way to improve this.
    // So far we've been passing args by value to Print function. This means that their copies are made.
    // Improvement: pass them by const reference.

    Print8(1, "test", 3.14);
    // Output:
    // 1, test, 3.14

    std::cout << "\nPrint8()" << std::endl;
    perfect_forwarding::Integer n{56};
    Print8(1, "test", 3.14, n, perfect_forwarding::Integer{67});
    // Output:
    // <-- Not ideal: copy c-tor called although we passed temp object. Let's pass by rvalue ref!
    // Integer(int n) // Creating n
    // Integer(int n) // Creating temp object.
    // 1, test, 3.14, 56, 67
    // ~Integer()

    std::cout << "\nPrint9()" << std::endl;
    Print9(1, "test", 3.14, n, perfect_forwarding::Integer{67});
    // Output:
    // <-- In a recursive call, as args is a name of a temp variable,
    // rvalue ref collapses to lvalue ref so copy c-tor is still called. Let's use perfect forwarding to the rescue!
    // Integer(int n)
    // 1, test, 3.14, 56, 67
    // ~Integer()

    std::cout << "\nPrint10()" << std::endl;
    Print10(1, "test", 3.14, n, perfect_forwarding::Integer{67});
    // Output:
    // Integer(int n)
    // 1, test, 3.14, 56, 67
    // ~Integer()
    // ~Integer() // destroying n
}

void demo(){
    // show_problem();
    show_solution();
}
}

// Complete Modern C++ (C++11/14/17) course by Umar Lone
// https://www.udemy.com/course/beg-modern-cpp/

// Create a factory method that creates an instance of some type T, initializes it with arguments and returns the instance.
// It should work with any type that accepts arbitrary number of arguments.
// Here are some usage examples of the factory.
//
// Employee * emp = CreateObject<Employee>(
// "Bob",    //Name
// 101,      //Id
// 1000) ;   //Salary
//
// Contact *p = CreateObject<Contact>(
// "Joey",                //Name
// 987654321,             //Phone number
// "Boulevard Road, Sgr", //Address
// "joey@poash.com") ;    //Email

namespace assignment1 {

template<typename TObject, typename ...Params>
TObject* CreateObject(Params...args){
    return new TObject(args...);
}

class Employee {
public:
    Employee(std::string name, int id, int salary) {
        std::cout << "Employee: " << name << ", " << id << ", " << salary << std::endl;
    }
};

class Contact {
public:
    Contact(std::string name, long int phone, std::string address, std::string email) {
        std::cout << "Contact: " << name << ", " << phone << ", " << address << ", " << email << std::endl;
    }
};

void demo() {
    Employee * emp = CreateObject<Employee>(
        "Bob",    //Name
        101,      //Id
        1000) ;   //Salary

    // Employee: Bob, 101, 1000

    Contact *p = CreateObject<Contact>(
        "Joey",                //Name
        987654321,             //Phone number
        "Boulevard Road, Sgr", //Address
        "joey@poash.com") ;    //Email

    // Contact: Joey, 987654321, Boulevard Road, Sgr, joey@poash.com
}

}

// Class Templates
//
// - used so classes can handle different types
// - typically used by containers
// - C++ standard library contains several containers that are implemented through class templates (vector, list, set, map...)
namespace class_templates {

namespace problem {

// We assume this container class can hold only integers.
class Stack {
    int buff_[512];
    int top_{-1};
public:
    void Push(int n) {
        buff_[++top_] = n;
    }

    void Pop() {
        --top_;
    }

    int Top() {
        return buff_[top_];
    }

    bool IsEmpty() {
        return top_ == -1;
    }
};

void show() {
    Stack stack;
    stack.Push(1);
    stack.Push(2);
    stack.Push(3);
    stack.Push(4);

    while (!stack.IsEmpty()) {
        std::cout << stack.Top() << std::endl;
        stack.Pop();
    }

    // Problem:
    // What if we want to store elements of type float, double, string?
}

} // namespace problem

namespace solution {

// compiler will create only classes for those types that template is instantiated for.
// Only those member functions that are invoked will be instantiated.
template <typename T>
class Stack {
    T buff_[512];
    int top_{-1};
public:
    // T can be a user-defined type so we should pass it by const reference
    void Push(const T& elem) {
        buff_[++top_] = elem;
    }

    void Pop() {
        --top_;
    }

    const T& Top() {
        return buff_[top_];
    }

    bool IsEmpty() {
        return top_ == -1;
    }
};

// class templates can accept non-type template arguments
template <typename T, int size>
class Stack2 {
    T buff_[size];
    int top_{-1};
public:
    // T can be a user-defined type so we should pass it by const reference
    void Push(const T& elem) {
        buff_[++top_] = elem;
    }

    // We can define memeber function outside the class definition
    void Pop();

    const T& Top() {
        return buff_[top_];
    }

    bool IsEmpty() {
        return top_ == -1;
    }

    // A factory method - creates an instance of the class
    // Return type does not need complete template type parameters as this definition is within the template class.
    // This is so called "shorthand notation" in which we don't need to specify template type parameters.
    // It can be used only if that type is inside class definition. It cannot be used outside the class.
    static Stack2 Create() {
        return Stack2<T, size>(); // temp object returned by value
    }

    // We can define factory method outside the class definition.
    static Stack2 Create2();

    Stack2() = default;

    // Copy constructor; its argument uses the shorthand notation.
    // Long-hand notation can also be used but is not required.
    Stack2(const Stack2& other) {
        for (int i = 0; i <= other.top_; ++i) {
            buff_[i] = other.buff_[i];
        }
        top_ = other.top_;
    }
};

// If we implement member functions outside the class declaration, we need to follow the syntax of the templates
// In templates, template parameters are part of the type, therefore we need to write Stack2<T, size>:
template<typename T, int size>
void Stack2<T, size>::Pop() {
    --top_;
}

// We can have Factory method definition outside the class
template<typename T, int size>
Stack2<T, size> Stack2<T, size>::Create2() {
    return Stack2<T, size>();
}


void show() {

    {
        Stack<int> stack;
        stack.Push(1);
        stack.Push(2);
        stack.Push(3);
        stack.Push(4);

        while (!stack.IsEmpty()) {
            std::cout << stack.Top() << std::endl;
            stack.Pop();
        }
    }

    {
        Stack<float> stack;
        stack.Push(1.);
        stack.Push(2);
        stack.Push(3);
        stack.Push(4);

        while (!stack.IsEmpty()) {
            std::cout << stack.Top() << std::endl;
            stack.Pop();
        }
    }

    {
        Stack2<int, 10> stack;
        stack.Push(1);
        stack.Push(2);
        stack.Push(3);
        stack.Push(4);

        while (!stack.IsEmpty()) {
            std::cout << stack.Top() << std::endl;
            stack.Pop();
        }
    }

    {
        // error: conversion from ‘Stack2<[...],10>’ to non-scalar type ‘Stack2<[...],9>’ requested
        // Stack2<int, 9> stack = Stack2<int, 10>::Create();
    }

    {
        Stack2<int, 10> stack;
        stack.Push(12);
        stack.Push(23);
        stack.Push(34);
        stack.Push(45);

        while (!stack.IsEmpty()) {
            std::cout << stack.Top() << std::endl;
            stack.Pop();
        }

        Stack2<int, 10> stack2(stack);
        while (!stack2.IsEmpty()) {
            std::cout << stack2.Top() << std::endl;
            stack2.Pop();
        }
    }

}

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
    // non_type_template_arguments::demo();
    // perfect_forwarding::demo();
    // variadic_templates::demo();
    // assignment1::demo();
    // class_templates::problem::show();
    class_templates::solution::show();
}

}
