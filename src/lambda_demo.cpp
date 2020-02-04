#include <lambda_demo.hpp>
#include <iostream>
#include <cassert>

namespace lambda_demo {

class S {
    int n_;
public:
    S(int n):n_(n){
        std::cout << "S::S()" << std::endl;
    };

    // copy constructor is a member function which initializes an object using another object of the same class
    S(const S&) {
         std::cout << "S::S(const S&)" << std::endl;
    }

    ~S(){
        std::cout << "S::~S()" << std::endl;
    }

    int GetValue() const {
        return n_;
    }

    void SetValue(int n) {
        n_ = n;
    }
};

// [] is called "capture clause"
// If capturing by value, you will get the value of the variable at the time the lambda is created, similar to passing a parameter to a function by value.
// If capturing by reference, you will have a reference to the actual variable outside the lambda (and you need to make sure it remains in scope).
// Note that inside a class you can capture "this" and then call class methods as you would in a class method.
void lambda_demo() {

    // An empty capture clause, [ ], indicates that the body of the lambda expression accesses no variables in the enclosing scope.
    auto l1 = [](int n){
        std::cout << "Hello from lambda which got int argument " << n << std::endl;
    };

    l1(3);

    int n1 = 7;
    auto l2 = [n1](int n) -> int {
        std::cout << "Hello from lambda which got int argument = " << n << " and which captured n1 = " << n1 << std::endl;
        return n + n1;
    };

    int res = l2(3);
    assert(res == 10);

    S s(55);
    std::cout << "Instance of S created. s.GetValue() = " << s.GetValue() << std::endl;

    // captures variable at the time of declaration
    auto lambdaCapturesByVal = [s]{
        std::cout << "Hello from lambda which captured s by value. s.GetValue() = " << s.GetValue() << std::endl;
    };

    auto lambdaCapturesByRef = [&s]{
        std::cout << "Hello from lambda which captured s by reference. s.GetValue() = " << s.GetValue() << std::endl;
    };

    s.SetValue(56);

    lambdaCapturesByVal(); // prints: s.GetValue() = 32701
    lambdaCapturesByRef(); // prints: s.GetValue() = 56
}

void run() {
    std::cout << "lambda_demo::run()" << std::endl;
    lambda_demo();
}

}
