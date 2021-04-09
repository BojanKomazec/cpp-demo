#include <std_string_view_demo.hpp>
#include <iostream>


// override operator new (must be in global namespace) so we can log any memory allocations
void* operator new(std::size_t count){
    std::cout << "   " << count << " bytes" << std::endl;
    return malloc(count);
}

namespace std_string_view_demo {

// error: ‘void* std_string_view_demo::operator new(std::size_t)’ may not be declared within a namespace
// (see https://stackoverflow.com/questions/6210921/operator-new-inside-namespace)
// void* operator new(std::size_t count){}

//
// SSO (Short String Optimization)
//
// https://stackoverflow.com/questions/10315041/meaning-of-acronym-sso-in-the-context-of-stdstring/
// https://stackoverflow.com/questions/21694302/what-are-the-mechanics-of-short-string-optimization-in-libc/
// https://www.modernescpp.com/index.php/c-17-avoid-copying-with-std-string-view
//
// std::string string type stores its data on the heap only if the string exceeds an implementation-dependent size.
// This size for std::string is 15 for MSVC and GCC and 23 for Clang.
// GCC5 started supporting small string optimzation.
// That means on the contrary, small strings are stored directly in the string object. Therefore, no memory allocation is required.
//
// https://stackoverflow.com/questions/4970823/where-in-memory-are-string-literals-stack-heap
//
void instantiation_demo() {
    std::string s1("123456789012345");
    // no operator new output => no memory allocation took place!
    // 15 bytes + 1 byte for null-termination character

    std::string s3("1234567890123456");
    // operator new output: 17 bytes => memory allocated on the heap

    std::string_view sw1("123456789012345");
    // no operator new output => no memory allocation took place!
    std::cout << sw1 << std::endl;

    std::string_view sw2("1234567890123456");
    // no operator new output => no memory allocation took place (even for the long string!)
    std::cout << sw2 << std::endl;
}

// Function just needs to read the string, it does not want to take the ownership.
// std::string is created from the string literal
std::size_t get_length_of_string(const std::string& str) {
    return str.length();
}

// Function just needs to read the string, it does not want to take the ownership.
// std::string_view is created from the string literal
std::size_t get_length_of_string_view(const std::string_view& str_view) {
    return str_view.length();
}

void function_read_string_demo() {
    std::cout << "About to call get_length_of_string()" << std::endl;
    auto len = get_length_of_string("1234567890123456");
    std::cout << "String length = " << len << std::endl;

    std::cout << "About to call get_length_of_string_view()" << std::endl;
    len = get_length_of_string_view("1234567890123456");
    std::cout << "String view length = " << len << std::endl;
}
//
// Console output:
//
// About to call get_length_of_string()
//    17 bytes <-- memory allocated !
// String length = 16
// About to call get_length_of_string_view() <-- no memory allocation when passing as std::string_view
// String view length = 16
//
// Conclusion: for this type of functions it is more optimal to pass a reference to std::string_view.
// https://softwareengineering.stackexchange.com/questions/364093/when-should-i-use-string-view-in-an-interface
//

void demo() {
    // instantiation_demo();
    function_read_string_demo();
}

void run() {
    std::cout << "std_string_view_demo::run()" << std::endl;
    demo();
}

}
