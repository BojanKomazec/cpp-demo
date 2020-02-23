#include <lambda_demo.hpp>
#include <iostream>
#include <cassert>

namespace preprocessor_demo {

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

void run() {
    std::cout << "preprocessor_demo::run()" << std::endl;
    demo();
}

}
