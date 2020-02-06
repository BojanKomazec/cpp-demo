#include <statements_demo.hpp>
#include <cassert>
#include <iostream>
#include <typeinfo>
#include <vector>

namespace statements_demo {

class S {
    int n_;
public:
    S(int n): n_(n) {
        std::cout << "S::S(int)" << std::endl;
    }

    S(const S& s): n_(s.n_) {
        std::cout << "S::S(const S& s)" << std::endl;
    }

    int GetValue() const { return n_; }
    void SetValue(int n) { n_ = n; }
};

// can be used on any collection which implements C++ "range" protocol (via indexes or iterators)
void range_based_for_loop_demo() {
    int arr[] {1, 2, 3};
    std::cout << "arr elements: ";
    for (auto el : arr) {
        std::cout << el << " ";
    }
    std::cout << std::endl;

    std::cout << "Initializing vector..." << std::endl;
    std::vector<S> v {S(1), S(2), S(3)};

    std::cout << "Iterating through vector..." << std::endl;
    std::cout << "v elements: \n";
    for (auto el : v) {
        // notice in log how copy c-tor is called for each element - a new value in memory is created!
        std::cout << el.GetValue() << "\n";
    }
    std::cout << std::endl;

    std::cout << "Iterating through vector (auto&)..." << std::endl;
    std::cout << "v elements: \n";
    for (auto& el : v) {
        // notice in log that copy c-tor is NOT called - only references are used!
        std::cout << el.GetValue() << "\n";
        // also, as ref is declared as non-const, we can modify element through it:
        el.SetValue(el.GetValue() + 10);
        std::cout << "New element value: " << el.GetValue() << "\n";
    }
    std::cout << std::endl;

    // To prevent modifying elements in the loop body, we need to declare el as const reference:
    std::cout << "Iterating through vector (const auto&)..." << std::endl;
    std::cout << "v elements: \n";
    for (const auto& el : v) {
        // notice in log that copy c-tor is NOT called - only references are used!
        std::cout << el.GetValue() << "\n";
        // also, as ref is declared as const, we cannot modify element through it:
        // error: passing ‘const statements_demo::S’ as ‘this’ argument discards qualifiers [-fpermissive]
        // el.SetValue(44);
        // std::cout << "New element value: " << el.GetValue() << "\n";
    }
    std::cout << std::endl;

    // initializer list implements range protocol
    std::cout << "Iterating through initializer list...\n";
    for( auto n : {11, 22, 33, 44}) {
        std::cout << n << "\n";
    }
    std::cout << std::endl;

    // If we need to know/use the index, then we need to use classic for-loop:
    std::cout << "Iterating through vector with classic for loop..." << std::endl;
    for (auto i = 0; i < v.size(); ++i) {
        std::cout << "v[" << i << "] = " << v.at(i).GetValue() << "\n";
    }
    std::cout << std::endl;
}

void run(){
    std::cout << "statements_demo::run()" << std::endl;
    range_based_for_loop_demo();
}

}
