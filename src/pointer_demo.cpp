#include <pointer_demo.hpp>
#include <iostream>
#include <cassert>

namespace pointer_demo {

    void pointer_demo(){
        int *pInt1, *pInt2, n1;

        // uninitialized pointer contains a random memory address
        std::cout << "pInt1 = " << pInt1 << std::endl;

        n1 = 1;
        std::cout  << "n1 = " << n1 << "; &n1 = " << &n1 << std::endl;
        pInt1 = &n1;
        std::cout << "pInt1 = " << pInt1 << std::endl;

        // T* pointer must point to variable of type T
        // error: cannot convert ‘int*’ to ‘float*’ in initialization
        // float* pFloat = pInt1;

        // void* can point to variable of any type
        void* pVoid = pInt1;
        std::cout << "pVoid = " << pVoid << std::endl;

        // * - dereference value
        *pInt1 = 2; // assign value via pointer
        std::cout  << "n1 = " << n1 << std::endl;

        int n2 = *pInt1; // read value via pointer
        std::cout  << "n2 = " << n2 << std::endl;

        // null pointer
        pInt2 = NULL; // C and pre C++11
        pInt2 = 0;
        pInt2 = nullptr; // C++11
        std::cout << "pInt2 = " << pInt2 << std::endl;

        // it is illegal to read from / write to nullptr
        // this compiles but throws error in runtime: Segmentation fault (core dumped)
        // *pInt2 = 3;
        // this compiles but throws error in runtime: Segmentation fault (core dumped)
        // n2 = *pInt2;
    }

    // Add two numbers and return the sum
    int Add(int *a, int *b) {
        return *a + *b;
    }

    // Add two numbers and return the sum through the third pointer argument
    void AddVal(int *a, int *b, int *result) {
        *result = *a + *b;
    }

    // Swap the value of two integers
    void Swap(int *a, int *b) {
        int temp;
        temp = *a;
        *a = *b;
        *b = temp;
    }

    // Generate the factorial of a number and return that through the second pointer argument
    void Factorial(int *a, int *result) {
        *result = 1;
        for (int i = 1; i <= *a; ++i) {
            *result *= i;
        }
    }

    void test_algos() {
        int n1 = 2;
        int n2 = 3;
        int n3;
        assert(Add(&n1, &n2) == 5);
        AddVal(&n1, &n2, &n3);
        assert(n3 == 5);
        Swap(&n1, &n2);
        assert(n1 == 3 && n2 == 2);
        Factorial(&n1, &n3);
        assert(n3 == 6);
    }

    void run() {
        std::cout << "pointer_demo::run()" << std::endl;
        pointer_demo();
        test_algos();
    }
}
