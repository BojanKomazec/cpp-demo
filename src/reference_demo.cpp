#include <reference_demo.hpp>
#include <iostream>
#include <cassert>

namespace reference_demo {

    void reference_demo(){

        // referent
        int n1 = 1;

        // error: ‘rn11’ declared as reference but not initialized
        // int& rn11;

        // reference; must be initialized
        int& rn1 = n1;

        // rs1 is a reference to non-const pointer to const char
        // error: cannot bind non-const lvalue reference of type ‘const char*&’ to an rvalue of type ‘const char*’
        // const char*& rs1 = "test";

        // rs1 is a reference to const pointer to const char
        const char* const& rs1 = "test";
        std::cout << "rs1 = " << rs1 << std::endl;

        // error: cannot bind non-const lvalue reference of type ‘void*&’ to an rvalue of type ‘void*’
        // void*& rpVoid = nullptr;

        void* const & rpVoid = nullptr;


        // reference *is* the referent; no new memory is allocated for reference
        assert(&rn1 == &n1);

        // reference *is* the referent, so changing the reference changes the state of the referent
        rn1 = 2;
        assert(n1 == 2);
    }

    // Add two numbers and return the result through a reference parameter
    void Add(int a, int b, int &result) {
        result = a + b;
    }

    // Find factorial of a number and return that through a reference parameter
    void Factorial(int a, int &result) {
        result = 1;
        for (int i = 1; i <= a; ++i) {
            result *= i;
        }
    }

    // Swap two numbers through reference arguments
    void Swap(int &a, int &b) {
        int temp = a;
        a = b;
        b = temp;
    }

    void test_algos() {
        int n1 = 2;
        int n2 = 3;
        int n3;

        int& rn3 = n3;
        Add(n1, n2, rn3);
        assert(n3 == 5);

        Swap(n1, n2);
        assert(n1 == 3 && n2 == 2);

        Factorial(3, rn3);
        assert(n3 == 6);
    }

    void run() {
        std::cout << "reference_demo::run()" << std::endl;
        reference_demo();
        test_algos();
    }
}
