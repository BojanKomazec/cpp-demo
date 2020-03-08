#include <initialization_demo.hpp>
#include <iostream>
#include <vector>
#include <cassert>
#include <initializer_list>

namespace initialization_demo{

//
// Uniform Initialization
//
// - Works at compile time.
// - Provides uniform initialization for scalar types and arrays.
// - Scalar types are initialized in the same way as vector types - with initializer list.
// - It automatically initializes the variable to 0 (or pointers to nullptr), thereby preventing runtime bugs.
// - It introduced a new class, std::intializer_list, which is used to store an array of objects.
//
// https://en.cppreference.com/w/cpp/language/list_initialization
// https://stackoverflow.com/questions/21825933/any-difference-between-copy-list-initialization-and-traditional-copy-initializat
//
void list_initialization_demo() {

    // value-initialization
    // the variable is initialized with the default value of the given type
    int n1{};
    assert(n1 == 0);

    // direct-list-initialization
    int n2{123};
    assert(n2 == 123);

    char ch2 {'w'};
    assert(ch2 == 'w');

    float f3 {1.41f};
    assert(f3 == 1.41f);

    double d1 {1.41};
    assert(d1 == 1.41);

    // copy-list-initialization
    int n3 = {};
    assert(n3 == 0);

    // copy-list-initialization
    int n4 = {0};
    assert(n4 == 0);

    // warning: implicit conversion from 'int' to 'char' changes value from 129 to -127 [-Wconstant-conversion]
    char ch1 = 129;

    // error: constant expression evaluates to 129 which cannot be narrowed to type 'char' [-Wc++11-narrowing]
    // char ch2 = { 129 };

    double d = 12.34;
    float f1 = d;

    // error: non-constant-expression cannot be narrowed from type 'double' to 'float' in initializer list [-Wc++11-narrowing]
    // float f2 = { d };

    // OK
    float f2 = { 12.34 };

    // warning: implicit conversion from 'double' to 'int' changes value from 12.34 to 12 [-Wliteral-conversion]
    int i1 = 12.34;

    int i2 = f2;
    assert(i2 == 12);

    //
    // initializer list detects "Narrowing of data"
    //

    // error: type 'double' cannot be narrowed to 'int' in initializer list [-Wc++11-narrowing]
    // error: narrowing conversion of ‘1.234e+1’ from ‘double’ to ‘int’ inside { } [-Wnarrowing]
    // int i2 = { 12.34 };

    // warning: narrowing conversion of ‘f2’ from ‘float’ to ‘int’ inside { } [-Wnarrowing]
    int i3{f2};

    //  warning: narrowing conversion of ‘i3’ from ‘int’ to ‘float’ inside { } [-Wnarrowing]
    float f4{i3};
    assert(f4 == 12.0f);

    //
    // uniform initialization also works for user-defined types
    //
    std::string s {"test"};

    // arr1 will be of type: int T[5]
    // This worked even before C++11
    int arr1[] = {1, 2, 3, 4, 5};
    // this prints: arr1 = 0x7ffd47a329b0 (array decays to pointer)
    std::cout << "arr1 = " << arr1 << std::endl;

    // This is possible from C++11
    // That's why using {} intializer list is called Uniform initialization - scalars, arrays and vectors can be initialized with the same construct.
    // Behind the scenes, whenver compiler sees {...} it creates an instance of std::initialzer_list<T>
    std::vector<int> v1 = {1, 2, 3, 4, 5};

    std::cout << "Vector v1: ";
    for (int i : v1) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    int arr2[3];
    // You cannot assign directly to an array after its declaration:
    // error: assigning to an array from an initializer list
    // arr2 = {6, 7, 8};
    // @todo: use std::copy() to assign initializer list elements to array

    //std::initializer_list<int> il1 = {1, 3, 5, 7};
    //std::cout << "il1 = " << il1 << std::endl;

    std::cout << "arr3 elements: " << "\n";
    int arr3[]{1, 2, 3};
    std::cout << "arr3:" << "\n";
    for(auto i : arr3) {
        std::cout << i << "\n";
    }

    int arr4[] = {4, 5, 6, 7};
    std::cout << "arr4:" << "\n";
    for(auto i : arr4) {
        std::cout << i << "\n";
    }

    int* pInt {};
    assert(pInt == nullptr);
}

//
// std::initializer_list
//
// - Has nothing to do with member intializations lists.
// - Introduced by Uniform Initialization.
// - Used to store an array of objects.
// - A lightweight proxy object that represents an array of objects.
// - It is a class template.
// - Commonly used with container classes.
//   Container classes are those that hold objects of other classes.
// - Constructed automatially from a braced list of elements.
// - Not a true container but has similar behaviour.
// - Provides access to its elements via iterators.
// - Declared in header <initializer_list>
namespace initilaizer_list_demo {

// An example of Custom container type.
//
// E.g. it contains a collection of orderIDs which are integers.
// Collection can have up to 10 IDs.
class Orders {
    int ids_[10];
    int size_{};
public:
    Orders(){}

    // This c-tor allows intializing user-defined container-type class with initializer list.
    Orders(std::initializer_list<int> init_list) {
        // we can check the size of the initializer list:
        assert(init_list.size() < 10);

        // To access individual elements of the array we'll be using iterators.
        // Iterators work like a pointers to array.
        // Initialize iterator to the first element in the list
        auto it = init_list.begin();
        while(it != init_list.end()) {
            add(*it);
            ++it;
        }
    }

    void add(int id) {
        assert(size_ < 10);
        ids_[size_++] = id;
    }

    int remove_end() {
        assert(size_ > 0);
        return ids_[--size_];
    }

    int operator[](int index) const {
        return ids_[index];
    }

    int get_size() const {
        return size_;
    }
};

template<typename T>
void print_initializer_list(std::initializer_list<T> il) {
    auto it = il.begin();
    while(it != il.end()) {
        std::cout << *it << std::endl;
        ++it;
    }
}

// Implementation with range-based for loop.
// This loop requires iterators in order to operate.
template<typename T>
void print_initializer_list2(std::initializer_list<T> il) {
    for(auto element_value : il) {
        std::cout << element_value << std::endl;
    }
}

void demo() {
    // use brace-list of elements - it automatically creates initializer list
    std::initializer_list<int> il1 = {1, 2, 3, 4, 5};

    // type of il2 is std::initializer_list
    auto il2 = {6, 7, 8};

    Orders orders;
    orders.add(12);
    orders.add(34);
    orders.add(56);
    for (int i = 0; i < orders.get_size(); ++i) {
        std::cout << "Order id = " << orders[i] << std::endl;
    }

    // Adding IDs one by one is old-skool apprach. How can we add all initial IDs at once?
    // We can use initializer list but we need to add a c-tor to Orders which accepts it.
    Orders orders2 {12, 34, 56};
    for (int i = 0; i < orders.get_size(); ++i) {
        std::cout << "Order id = " << orders[i] << std::endl;
    }

    print_initializer_list<int>({11, 22, 33});
    print_initializer_list2<int>({44, 55, 66});

    for(auto element_value : {77, 88, 99}) {
        std::cout << element_value << std::endl;
    }
}

} // namespace initilaizer_list_demo

void run() {
    std::cout << "\n\n ***** initialization_demo::run() ***** \n\n" << std::endl;
    list_initialization_demo();
    initilaizer_list_demo::demo();
}

}
