#include <exceptions_demo.hpp>
#include <iostream>
#include <cassert>
#include <limits>
#include <new>          // std::bad_alloc
#include <memory>
#include <vector>
#include <random>

namespace exceptions_demo {

void process_records(int count){
    int* pArray = (int*)malloc(count * sizeof(int));

    if (pArray == nullptr) {
        std::cout << "malloc() failed to allocate requested memory." << std::endl;
        return;
    }

    for (int i = 0; i < count; ++i) {
        pArray[i] = i;
    };

    free(pArray);
}


int process_records2(int count){
    int* pArray = (int*)malloc(count * sizeof(int));

    if (pArray == nullptr) {
        std::cout << "malloc() failed to allocate requested memory." << std::endl;
        return 1;
    }

    for (int i = 0; i < count; ++i) {
        pArray[i] = i;
    };

    free(pArray);

    return 0;
}

//
// Exception Handling
//
// - mechanism to handle errors in programs that occur in runtime.
// - these error are called exceptions
// - exist outside the normal functioning of the program
// - requires immediate handling by the program
// - if not handled, the program crashes
// - cannot be ignrored, unlike C error handling
//
// try
//
// - creates a scope/block inside which is the code which throws exception
// - can contain other try-catch statements
//
// throw
//
// - throws an exception from the try block
// - exception is object that is constructed in throw statement
//
// catch
//
// - handler that catches the exception object
// - should appear just after the try block
// - multiple catch blocks can exist - for each type of exception that can be thrown from try block.
// - contains code which handles the caught exception
// - for each exception thrown there should be matching catch otherwise excepton might be unhandled.
// - If catch handler accepts the base type of exception object it can catch any of its children (by ref).
// - All standard exception classes are inherited from std::exception so we can have a single catch block
//   which accepts std::exception&.

void process_records3(int count){
    int* pArray = (int*)malloc(count * sizeof(int));

    if (pArray == nullptr) {
        throw std::runtime_error("malloc() failed to allocate requested memory.");
    }

    for (int i = 0; i < count; ++i) {
        pArray[i] = i;
    };

    free(pArray);
}

void process_records4(int count){
    if (count < 10) {
        throw std::out_of_range("Count should be >= 10");
    }

    int* pArray = (int*)malloc(count * sizeof(int));

    if (pArray == nullptr) {
        throw std::runtime_error("malloc() failed to allocate requested memory.");
    }

    for (int i = 0; i < count; ++i) {
        pArray[i] = i;
    };

    free(pArray);
}

void process_records5(unsigned int count){
    if (count < 10) {
        throw std::out_of_range("Count should be >= 10");
    }

    // throws std::bad_alloc in case of failure
    int* pArray = new int[count * sizeof(int)];

    for (int i = 0; i < count; ++i) {
        pArray[i] = i;
    };

    delete[](pArray);
}

void problem_and_solution_demo() {
    // we want to make malloc to fail
    process_records(std::numeric_limits<unsigned int>::max());
    // output: malloc() failed to allocate requested memory.
    // Problem: caller does not know that process_records() actually failed.
    // Soulution #1: make process_records() returning error codes.

    process_records2(std::numeric_limits<unsigned int>::max());
    // Problem: caller does check return error code and it still does not know that process_records() actually failed.
    // Soulution #2: force caller not to ignore process_records() failure in some way.

    // process_records3(std::numeric_limits<unsigned int>::max());
    // If we don't put this in try block and handle it in catch then program will crash with message in output:
    //      terminate called after throwing an instance of 'std::runtime_error'
    //      what():  malloc() failed to allocate requested memory.
    //      Aborted (core dumped)

    try{
        process_records3(std::numeric_limits<unsigned int>::max());
    }catch(std::runtime_error& ex){
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    try{
        std::cout << "Calling process_records4()" << std::endl;
        process_records4(3);
    }catch(std::runtime_error& ex){
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }catch(std::out_of_range& ex){
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    try{
        std::cout << "Calling process_records5()" << std::endl;
        process_records5(std::numeric_limits<unsigned int>::max());
    }catch(std::runtime_error& ex){
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }catch(std::out_of_range& ex){
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }catch(std::bad_alloc& ex){
        std::cout << "Exception caught: " << ex.what() << std::endl;
        // output:
        //      Exception caught: std::bad_alloc
    }

    try{
        std::cout << "Calling process_records5()" << std::endl;
        process_records5(std::numeric_limits<unsigned int>::max());
    }catch(std::exception& ex){
        std::cout << "Exception caught: " << ex.what() << std::endl;
        // output:
        //      Exception caught: std::bad_alloc
    }

    try{
        std::cout << "Calling process_records5()" << std::endl;
        process_records5(std::numeric_limits<unsigned int>::max());
    }catch(...){ // all-catch block
        std::cout << "Exception caught." << std::endl;
        // Problem: it does not give information about the type of the exception.
        // This catch blocks should always be the last one in the list of catch blocks.
        // Catch handlers should be listed from those which catch most specific exceptions to
        // those which catch more generic exceptions.
        // output:
        //      Exception caught.
    }
}

class ResourceOwner{
public:
    ResourceOwner(){
        std::cout << "ResourceOwner::ResourceOwner()" << std::endl;
    }
    ~ResourceOwner(){
        std::cout << "ResourceOwner::~ResourceOwner()" << std::endl;
    }
};

void process_records6(unsigned int count){
    ResourceOwner ro;

    if (count < 10) {
        throw std::out_of_range("Count should be >= 10");
    }

    // throws std::bad_alloc in case of failure
    int* pArray = new int[count * sizeof(int)];

    for (int i = 0; i < count; ++i) {
        pArray[i] = i;
    };

    delete[](pArray);
}

void process_records7(unsigned int count){
    ResourceOwner* ro = new ResourceOwner();

    if (count < 10) {
        throw std::out_of_range("Count should be >= 10");
    }

    // throws std::bad_alloc in case of failure
    int* pArray = new int[count * sizeof(int)];

    for (int i = 0; i < count; ++i) {
        pArray[i] = i;
    };

    delete[](pArray);
    delete ro;
}


void process_records8(unsigned int count){
    std::unique_ptr<ResourceOwner> ro{new ResourceOwner()};

    if (count < 10) {
        throw std::out_of_range("Count should be >= 10");
    }

    // throws std::bad_alloc in case of failure
    int* pArray = new int[count * sizeof(int)];

    for (int i = 0; i < count; ++i) {
        pArray[i] = i;
    };

    delete[](pArray);
}

void process_records9(unsigned int count){
    std::unique_ptr<ResourceOwner> ro{new ResourceOwner()};

    if (count < 10) {
        throw std::out_of_range("Count should be >= 10");
    }

    std::vector<int> v;
    v.reserve(count);

    for (int i = 0; i < count; ++i) {
        v.push_back(i);
    };
}

void stack_unwinding_demo(){

    {
        try{
            std::cout << "Calling process_records6()" << std::endl;
            process_records6(std::numeric_limits<unsigned int>::max());
        }catch(std::exception& ex){
            std::cout << "Exception caught: " << ex.what() << std::endl;
        }

        // Output:
        // Calling process_records6()
        // ResourceOwner::ResourceOwner()
        // ResourceOwner::~ResourceOwner()
        // Exception caught: std::bad_alloc
        //
        // This shows that when function throws an exception the stack of the function is properly destroyed (unwinded).
        // All local objects are destroyed, their destructors are called.
        // This process is called Stack Unwinding.
        // It will destroy only local objects on the stack.
        // If objects are allocated on the heap, they will not be destroyed and memory leak can happen.
    }

    {
        try{
            std::cout << "Calling process_records7()" << std::endl;
            process_records7(std::numeric_limits<unsigned int>::max());
        }catch(std::exception& ex){
            std::cout << "Exception caught: " << ex.what() << std::endl;
        }
        // output:
        // Calling process_records7()
        // ResourceOwner::ResourceOwner()
        // Exception caught: std::bad_alloc
        //
        // Problem: ResourceOwner::~ResourceOwner() is not called.
        // Solution: use RAII - use smart pointers!
    }

    {
        try{
            std::cout << "Calling process_records8()" << std::endl;
            process_records8(std::numeric_limits<unsigned int>::max());
        }catch(std::exception& ex){
            std::cout << "Exception caught: " << ex.what() << std::endl;
        }
        // output:
        // Calling process_records8()
        // ResourceOwner::ResourceOwner()
        // ResourceOwner::~ResourceOwner()
        // Exception caught: std::bad_alloc
        //
        // Problem: pArray is not deleted.
        // Solution: use std::vector.
    }


    {
        try{
            std::cout << "Calling process_records9()" << std::endl;
            process_records9(10);
        }catch(std::exception& ex){
            std::cout << "Exception caught: " << ex.what() << std::endl;
        }
        // output:
        // Calling process_records9()
        // ResourceOwner::ResourceOwner()
        // ResourceOwner::~ResourceOwner()
    }
}

void process_records10(unsigned int count){
    std::unique_ptr<ResourceOwner> ro{new ResourceOwner()};

    if (count < 10) {
        throw std::out_of_range("Count should be >= 10");
    }

    std::vector<int> v;
    v.reserve(count);

    for (int i = 0; i < count; ++i) {
        v.push_back(i);
    };

    std::default_random_engine eng;
    std::bernoulli_distribution dist;

    int errors{};

    for (int i = 0; i < count; ++i) {
        std::cout << "Processing record #" << i << std::endl;
        try {
            if (!dist(eng)) {
                ++errors;
                throw std::runtime_error("Could not process the record");
            }
        } catch(std::runtime_error& ex){
            std::cout << ex.what() << std::endl;
            if (errors > 3) {
                // re-throws the original exception object.
                // throw;

                // It is possible to modify it (works only if catching by reference):
                std::runtime_error rte("Too many errors. Stopping the processing...");
                ex = rte;
                throw;
            }
        }
    }
}

void nested_exceptions_demo() {
    try{
        std::cout << "Calling process_records10()" << std::endl;
        process_records10(10);
    }catch(std::exception& ex){
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }
}

class A {
public:
    A(){
        std::cout << "A::A()" << std::endl;
    }
    ~A(){
        std::cout << "A::~A()" << std::endl;
    }
};

class B {
public:
    B(){
        std::cout << "B::B()" << std::endl;
    }
    ~B(){
        std::cout << "B::~B()" << std::endl;
    }
};

// If c-tor throws exception, d-tor will not be executed => resources will not be releaased.
// b_ member will be deallocated normally - it's destructor will be called.
// For members which memory is manually allocated - we need to take care deallocating it in case of exception.
// Solution: RAII - using smart pointers.
class ResourceOwner2 {
    A* pA_{};
    B b_{};
    int* pInt_{};
    char* pStr_{};
    int* pArr_{};
public:
    ResourceOwner2(){
        std::cout << "ResourceOwner2::ResourceOwner2(): Acquiring resource..." << std::endl;
        pA_ = new A;
        pInt_ = new int;

        throw std::runtime_error("Failed to initialize");

        pStr_ = new char[100];
        pArr_ = new int[50];
    }
    ~ResourceOwner2(){
        std::cout << "ResourceOwner2::~ResourceOwner2(): Releasing resource..." << std::endl;
        delete pA_;
        delete pInt_;
        delete[] pStr_;
        delete[] pArr_;
    }
};

class ResourceOwner3 {
    std::unique_ptr<A> pA_{};
    B b_{};
    std::unique_ptr<int> pInt_{};
    std::string pStr_{};
    std::vector<int> pArr_{};
public:
    ResourceOwner3(){
        std::cout << "ResourceOwner3::ResourceOwner3(): Acquiring resource..." << std::endl;
        pA_.reset(new A);
        throw std::runtime_error("Failed to initialize");
    }

    // Destructors should not throw exceptions.
    // Destructor might be invoked during stack unwinding which might happen due to another exception being thrown.
    // If exception is thrown during stack unwinding, program will terminate.
    // If destructor throws an exception, this exception has to stay withing destructor, it has to be handled inside destructor.
    ~ResourceOwner3(){
        std::cout << "ResourceOwner3::~ResourceOwner3(): Releasing resource..." << std::endl;

        // This line:
        //      throw std::runtime_error("Failed to destruct");
        // would issue this warning:
        //      warning: throw will always call terminate() [-Wterminate]
    }
};


void throwing_exc_from_ctor_and_dtor_demo(){

    // showing problem: memory leak if c-tor throws
    try{
        std::cout << "Instantiating ResourceOwner2" << std::endl;
        ResourceOwner2 ro;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    // showing solution: ResourceOwner3 has smart pointers as members
    try{
        std::cout << "Instantiating ResourceOwner3" << std::endl;
        ResourceOwner3 ro;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

//
// noxcept (C++11)
//
// - specifier applied to functions (both in declaration and definition)
// - notifies compiler that function does not throw any exceptions => compiler
//   can optimize the function code as it does not need to generate stack
//   unwinding code.
// - if such function does throw an exception, program will terminate and stack might not be unwinded
// - functions which should not be specified as noexcept:
//   - functions calling other functions (in library)
// - Every function in C++ is either non-throwing or potentially throwing.
// - noexcept(true) means function cannot throw an exception
// - noexcept(false) means function can (may) throw an exception
// - use noexcept only if you're sure that function cannot throw
// - Idally, move c-tors are marked with noexcept(true) which helps clients decided whether to use move
//   or copy.
// - Destructors should also be marked with noexcept(true) which is implicitly the case from C++11.
// - If function may or may not throw an exception, it is 'exception neutral'.

void foo() {
    throw 1;
}

int sum(int a, int b) noexcept(true) {

    // warning: throw will always call terminate() [-Wterminate]
    // throw 1;
    // throwing from a noexcept function will cause this output:
    //      terminate called after throwing an instance of 'int'
    //      Aborted (core dumped)

    // foo();
    // Output:
    //      terminate called after throwing an instance of 'int'
    //      Aborted (core dumped)

    return a + b;
}

int sum2(int a, int b) noexcept(false) {
    throw 1;
    // foo();
    return a + b;
}

void bar() noexcept(false) {
    throw 1;
}

// bar2 is calling a library function and it can't guarantee that the this won't throw exception so
// in this case we need to specify bar2 as noexcept(false)
void bar2() noexcept(false) {
    std::cout << "bar2()" << std::endl;
}

// noexcept is also an operator which returns whether expression is noexcept or not, and it returns true or false.
// This can be used in order to automatically specify correct noexcept for a caller function.
int sum3(int a, int b) noexcept(noexcept(bar)) {
    foo();
    return a + b;
}

void noexcept_demo(){
    try {
        sum(1, 2);
    } catch (int x) {
        std::cout << "Exception: " << x << std::endl;
    }

    try {
        sum2(1, 2);
    } catch (int x) {
        std::cout << "Exception: " << x << std::endl;
    }

    std::cout << "bar: " << std::boolalpha << noexcept(bar) << std::endl; // true (be careful!)
    std::cout << "bar(): " << std::boolalpha << noexcept(bar()) << std::endl; // false

    A a;
    std::cout << "~A(): " << std::boolalpha << noexcept(a.~A()) << std::endl; //  true
    try {
        sum3(1, 2);
    } catch (int x) {
        std::cout << "Exception: " << x << std::endl;
    }
}


void run() {
    std::cout << "exceptions_demo::run()" << std::endl;
    // problem_and_solution_demo();
    // stack_unwinding_demo();
    // nested_exceptions_demo();
    // throwing_exc_from_ctor_and_dtor_demo();
    noexcept_demo();
}

}

