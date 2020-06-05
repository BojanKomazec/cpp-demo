#include <lambda_demo.hpp>
#include <iostream>
#include <cassert>

namespace preprocessor_demo {

#define ADD(X, Y) \
X + Y

#define DEFINE_ADD_FUNCTION \
int add(int i, int j) \
{ \
    return i + j; \
} \

// https://gcc.gnu.org/onlinedocs/cpp/Concatenation.html
// It is often useful to merge two tokens into one while expanding macros. This is called token pasting or token concatenation.
// The ‘##’ preprocessing operator performs token pasting.
// When a macro is expanded, the two tokens on either side of each ‘##’ operator are combined into a single token, which then
// replaces the ‘##’ and the two original tokens in the macro expansion.
// Token pasting is most useful when one or both of the tokens comes from a macro argument.
// If either of the tokens next to an ‘##’ is a parameter name, it is replaced by its actual argument before ‘##’ executes.
// As with stringizing, the actual argument is not macro-expanded first.
//
// In our case arg_name_prefix is replaced by argument passed which is 'arg' so name of 1st parameter will be arg1.
#define DEFINE_ADD2_FUNCTION(arg_name_prefix) \
int add2(int arg_name_prefix ## 1, int arg_name_prefix ## 2) \
{ \
    return arg_name_prefix ## 1 + arg_name_prefix ## 2; \
} \

#define DEFINE_PROCESS_FUNCTION(brand) \
void process() { \
    auto str = brand "_suffix"; \
    std::cout << "brand = " << str << std::endl; \
} \


void demo() {

#pragma message("Pragma message 1")
#pragma message "Pragma message 2"

#pragma message "Compiling " __FILE__ "..."

// ../cpp-demo/src/preprocessor_demo.cpp: In function ‘void preprocessor_demo::demo()’:
// <command-line>:0:31: note: #pragma message: MY_CUSTOM_PREPROCESSOR_SYMBOL defined. Value:SomeRandomValueOfMyCutomPreprocessorSymbol
// <command-line>:0:31: note: in definition of macro ‘MY_CUSTOM_PREPROCESSOR_SYMBOL’
#ifdef MY_CUSTOM_PREPROCESSOR_SYMBOL
    #if defined(PREPROCESSOR_DIAGNOSTICS)
        #pragma message "MY_CUSTOM_PREPROCESSOR_SYMBOL defined. Value:" MY_CUSTOM_PREPROCESSOR_SYMBOL
        // std::cout << "MY_CUSTOM_PREPROCESSOR_SYMBOL defined. Value: " << MY_CUSTOM_PREPROCESSOR_SYMBOL << std::end;
    #endif
#else
    #if defined(PREPROCESSOR_DIAGNOSTICS)
        pragma message "MY_CUSTOM_PREPROCESSOR_SYMBOL not defined"
        // std::cout << "MY_CUSTOM_PREPROCESSOR_SYMBOL not defined" << std::endl;
    #endif
#endif
}

DEFINE_ADD_FUNCTION

DEFINE_ADD2_FUNCTION(arg)

DEFINE_PROCESS_FUNCTION("Alma")

void macro_functions_demo(){
    auto res = ADD(1, 2);
    std::cout << "res = " << res << std::endl;
    // output:
    // res = 3
    std::cout << "ADD(1, 2) = " << ADD(1, 2) << std::endl;
    // output:
    // ADD(1, 2) = 3

    std::cout << "add(1, 2) = " << add(1, 2) << std::endl;
    // output:
    // add(1, 2) = 3

    std::cout << "add2(1, 2) = " << add2(1, 2) << std::endl;
    // output:
    // add2(1, 2) = 3

    process();
    // output:
    // brand = Alma_suffix
}

void run() {
    std::cout << "preprocessor_demo::run()" << std::endl;
    // demo();
    macro_functions_demo();
}

}
