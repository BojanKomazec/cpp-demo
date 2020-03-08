#include <std_vector_demo.hpp>
#include <iostream>
#include <cassert>
#include <vector>

namespace std_vector_demo {


// std::vector<T>
//
// - has behaviour of dynamic array and that's how it's implemented internally
// - is a class template; we need to specify what kind of objects we want it to store
void demo() {
    std::vector<int> vint;

    // can be initialized with initializer list (uniform initialization)
    std::vector<int> vint2{1, 2, 3};

    // adding new elements
    for (auto n : {4, 5, 6, 7, 8}) {
        vint2.push_back(n);
    }

    // access elements - read/write go via subscript operator
    vint2[0] = 100;
    for(int i = 0; i < vint2.size(); ++i) {
        std::cout << vint2[i] << std::endl;
    }

    // We can use range-based for loop which internally uses iterators;
    // All STL containers provide iterators so does vector.
    for (auto n : vint2) {
        std::cout << n << std::endl;
    }

    // begin() creates iterator and initializes it to the beginning of the dynamic array
    auto it = vint2.begin();
    // we can move iterator forwards:
    ++it;
    it += 5;
    // we can move iterator backwars:
    it -= 5;
    // We can access elements via iterator by using asterisk operator.
    // We can use to to read the value at the current iterator's position:
    std::cout << *it << std::endl;
    // ...or we can use it to write a new value to that element:
    *it = 7654;
    assert(*it == 7654);

    std::cout << std::endl;
    for (auto n : vint2) {
        std::cout << n << std::endl;
    }

    // removing an element
    std::cout << std::endl;
    it = vint2.begin();
    vint2.erase(it);
    for (auto n : vint2) {
        std::cout << n << std::endl;
    }

    // inserting an element
    std::cout << std::endl;
    it = vint2.begin();
    vint2.insert(it, 1212);
    for (auto n : vint2) {
        std::cout << n << std::endl;
    }
}

void run() {
    std::cout << "std_vector_demo::run()" << std::endl;
    demo();
}

}
