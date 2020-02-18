#include <utility_demo.hpp>
#include <iostream>
#include <utility>

namespace utility_demo {

class Integer {
    int* pVal_ {};
public:
    Integer() {
        std::cout << "Integer::Integer()" << std::endl;
        pVal_ = new int(0);
    }

    Integer(int n) {
        std::cout << "Integer::Integer(int). n = " << n << std::endl;
        pVal_ = new int(n);
    }

    Integer(const Integer& other) {
        std::cout << "Integer::Integer(const Integer&). other.GetValue() = " << other.GetValue() <<  std::endl;
        pVal_ = new int(other.GetValue());
    }

    Integer(Integer&& other) {
        std::cout << "Integer::Integer(Integer&&)" << std::endl;
        pVal_ = other.pVal_;

    }

    ~Integer() {
        std::cout << "Integer::~Integer()" << std::endl;
        delete pVal_;
    }

    void SetValue(int n) {
        delete pVal_;
        pVal_ = new int(n);
    }

    int GetValue() const {
        return *pVal_;
    }
};

void process(Integer n) {

}

// Adjacent string concatenation (C89/C++98)
// If two string literals are adjacent to each other with no punctuation in between, they are merged into a single string.
void move_demo() {
    std::cout << "move_demo()" << std::endl;

    // Integer::Integer(int). n = 1
    // Integer::Integer(const Integer&). other.GetValue() = 1
    // Integer::Integer(const Integer&). other.GetValue() = 1
    // Integer::~Integer()
    // Integer::~Integer()
    // Integer::~Integer()
    {
        // this is an Lvalue => c-tors that accept Lvalue will be invoked
        Integer n1(1);

        // copy c-tor called
        auto n2(n1);

        // copy c-tor called
        process(n1);
    }

    // What if we want to move state of n1 into another object?
    // How to enforce calling move c-tor?
    // We need to make Rvalue from Lvalue n1.
    // std::move() does exactly that. It forces move operation on Lvalues.
    // Also, some objects are non-copyable, but they can be moved through std::move. e.g. std::unique_ptr, std::thread

    // Integer::Integer(int). n = 1
    // Integer::Integer(Integer&&)
    // Integer::~Integer()
    // Integer::~Integer()
    {
        Integer n1(1);

        auto n2(std::move(n1));

        // WARNING! n2 now owns resource which used to be owned by n1.
        // n1 should not be used after std::move as that might cause undefined behavior.
        // std::cout << "n1.GetValue() = " <<  n1.GetValue() << std::endl;

        Integer n3(2);
        process(std::move(n3));
    }
}

void run() {
    std::cout << "\n\nutility_demo()\n\n" << std::endl;
    move_demo();
}

}
