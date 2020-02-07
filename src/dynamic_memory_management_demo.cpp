#include <dynamic_memory_management_demo.hpp>
#include <ios>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cstring> // strcpy_s

namespace dynamic_memory_management_demo {

// print each byte in new line
// *p = 65; // 0x41 0 0 0
void print_mem_content(const unsigned char* const p, const unsigned int n) {
    // std::ios_base::fmtflags f(std::cout.flags() );  // save flags state
    // std::cout << std::showbase << std::setfill('0') << std::setw(4); // set new flags

    for (auto i = 0; i < n; ++i) {
        unsigned char* currAddress = const_cast<unsigned char*>(p) + i;
        // std::cout << std::hex << currAddress << ": " << static_cast<int>(*(currAddress)) << "\n";
        std::cout << static_cast<void *>(currAddress) << ": " << std::hex << static_cast<int>(*(currAddress)) << std::dec << "\n";
    }

    std::cout << std::endl;

    // std::cout.flags(f);  // restore flags state
}

// https://en.wikipedia.org/wiki/C_dynamic_memory_allocation
// https://en.wikipedia.org/wiki/Dangling_pointer
// https://stackoverflow.com/questions/2902064/how-to-track-down-a-double-free-or-corruption-error
// http://www.cplusplus.com/reference/cstdlib/free/
//
// malloc()
//    g++ on my Ubuntu zeroes memory allocated by malloc although other compilers/OS can behave differently.
//    https://stackoverflow.com/questions/45323930/is-malloc-initializing-allocated-array-to-zero
// free(ptr)
//    If ptr does not point to a block of memory allocated with the above functions, it causes undefined behavior.
//    If ptr is a null pointer, the function does nothing.
//    free does not change the value of ptr itself, hence it still points to the same (now invalid) location.
void malloc_free_demo() {
    // error: invalid conversion from ‘void*’ to ‘int*’ [-fpermissive]
    // int* pInt = malloc(sizeof(int));

    // C-style cast; compiles but is not safe as compiler does not check types
    // int* pInt = (int*) malloc(sizeof(int));

    int* pInt =static_cast<int*>(malloc(sizeof(int)));

    // always check the return value of the function
    if (pInt == nullptr) {
        std::cout << "malloc() failed." << std::endl;
    }

    // error: invalid static_cast from type ‘int*’ to type ‘unsigned char*’
    // unsigned char* pUChar = static_cast<unsigned char*>(pInt);

    unsigned char* pUChar = reinterpret_cast<unsigned char*>(pInt);
    print_mem_content(pUChar, sizeof(int));

    *pInt = 65;
    assert(*pInt == 65);
    print_mem_content(pUChar, sizeof(int));

    free(pInt);
    print_mem_content(pUChar, sizeof(int));

    // pInt is now a "dangling pointer" - it contains an address of the freed memory block
    // free(pInt); // this might crash the app (it does not with /usr/bin/g++)

    // it is a good practice to set pointer to nullptr immediately after free()
    pInt == nullptr;

    // emulate calloc() with malloc()
    const auto elementsCount = 5;
    int* pInt2 =static_cast<int*>(malloc(elementsCount * sizeof(int)));
    if (pInt2 == nullptr) {
        std::cout << "malloc() failed." << std::endl;
    }

    // if we loose the address of the allocate memory before we free it, we won't be
    // able to free it later and this will cause a memory leak!
    // pInt2 = nullptr;

    free(pInt2);
    pInt2 == nullptr;
}

// calloc - Allocates and zero-initializes array
void calloc_free_demo() {
    const auto elementsCount = 1;
    int* pInt = static_cast<int*>(calloc(elementsCount, sizeof(int)));
    unsigned char* pUChar = reinterpret_cast<unsigned char*>(pInt);
    print_mem_content(pUChar, sizeof(int));
    if (pInt != nullptr) {
        free(pInt);
        pInt = nullptr;
    }
}

struct S {
    S() { std::cout << "S::S()" << std::endl; }
    ~S() { std::cout << "S::~S()" << std::endl; }
};

// int n = 27; <-- this places variable on the local stack
// Like functions malloc and calloc, operator 'new' allocates memory on the heap.
// Compiler knows the size of type T (the size of memory to be allocated).
// malloc/calloc can't intialize the value of the allocated memory; new can do that.
// malloc returns void* which needs to be casted; 'new' returns correct pointer type.
void new_delete_demo(){
    int* pInt = new int(27);
    assert(*pInt == 27);

    if (pInt != nullptr) {
        delete pInt;
    }

    // if new fails to allocate memory it throws an exception
    int* pInt2 = nullptr;
    try {
        pInt2 = new int(38);
    } catch (...) {
        std::cout << "new failed to allocate memory." << std::endl;
    }

    if (pInt2 != nullptr) {
        assert(*pInt2 == 38);
        delete pInt2;
        pInt2 = nullptr;
    }

    // malloc, calloc, realloc return NULL when fail to allocate memory.
    // new calls constructor, delete calls destructor
    // malloc/free don't do that
    S* pS = nullptr;
    try {
        pS = new S();
    } catch (...) {
        std::cout << "new failed to allocate memory." << std::endl;
    }

    if (pS != nullptr) {
        delete pS;
        pS = nullptr;
    }
}

void new_delete_for_arrays_demo(){
    const auto Size = 5; //array size
    int* pIntArr = new int[Size];
    for (int i = 0; i < Size; ++i) {
        pIntArr[i] = i + 1;
    }
    unsigned char* pUChar = reinterpret_cast<unsigned char*>(pIntArr);
    print_mem_content(pUChar, Size * sizeof(int));
    delete[] pIntArr;

    int* pIntArr2 = new int[Size]{1, 2, 3, 4, 5};
    delete[] pIntArr2;

    const char* msg = "test"; // 4 characters

    // allocate one more for null character \0
    char* pChar = new char[5];

    // https://stackoverflow.com/questions/40045973/strcpy-s-not-working-with-gcc
    // strcpy_s(pChar, 4, msg);
    strcpy(pChar, msg);
    std::cout << pChar << std::endl;
}

void two_dimensional_arrays_demo(){
    // 1st index is row, 2nd index is column
    int data[2][3] = {
        1, 2, 3,
        4, 5, 6
    };

    auto el = data[0][2]; // 1st row, 2nd column
    assert(el == 3);

    int* pRow1 = new int[3];
    int* pRow2 = new int[3];
    int** p2dArr = new int*[2];
    p2dArr[0] = pRow1;
    p2dArr[1] = pRow2;

    p2dArr[0][0] = 1;
    p2dArr[0][1] = 2;

    delete[] p2dArr[0];
    delete[] p2dArr[1];
    delete[] p2dArr;
}

void run() {
    std::cout << "dynamic_memory_management_demo::run()" << std::endl;
    malloc_free_demo();
    calloc_free_demo();
    new_delete_demo();
    new_delete_for_arrays_demo();
    two_dimensional_arrays_demo();
}

}
