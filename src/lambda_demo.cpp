#include <lambda_demo.hpp>
#include <iostream>
#include <cassert>
#include <functional>

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

void std_function_and_lambda_demo() {
    auto s1 = "This is a ";
    std::function<void(std::string_view const&)> fn{[s1](std::string_view const& sw){
        std::cout << "(lambda) Concatenation result = " << std::string(s1) + std::string(sw) << std::endl;
    }};

    fn("test");
    // Output:
    // (lambda) Concatenation result = This is a test
}


void event_int_operands_available(int op1, int op2, std::function<void(int, int)> callback){
    callback(op1, op2);
}

// lambdas are useful when defining callback/handler functions if they are used only at one place
// so we can avoid declaring (and naming) a whole new function so we can only use it in a single place.
void callback_lambda_demo() {
    event_int_operands_available(1, 2, [](int op1, int op2){
        std::cout << "(Lambda) The sum is " << op1 + op2 << std::endl;
    });

    // In the other call we want to use a different handler/callback.
    // A bit of English:
    // The terms of subtraction are called minuend and subtrahend, the outcome is called the difference.
    event_int_operands_available(1, 2, [](int op1, int op2){
        std::cout << "(Lambda) The difference is " << op1 - op2 << std::endl;
    });
}

void run() {
    std::cout << "lambda_demo::run()" << std::endl;
    // lambda_demo();
    std_function_and_lambda_demo();
    // callback_lambda_demo();
}

}
