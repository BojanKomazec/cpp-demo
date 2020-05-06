#include <lambda_demo.hpp>
#include <iostream>
#include <cassert>

namespace operators_demo {

class Integer {
    int* pVal_ {};
public:
    Integer() {
        std::cout << "Integer::Integer)" << std::endl;
        pVal_ = new int(0);
    }

    Integer(int n) {
        std::cout << "Integer::Integer(int). n = " << n << std::endl;
        pVal_ = new int(n);
    }

    Integer(const Integer& other) {
        std::cout << "Integer::Integer(const Integer). other.GetValue() = " << other.GetValue() <<  std::endl;
        pVal_ = new int(other.GetValue());
    }

    Integer(Integer&& other) {
        std::cout << "Integer::Integer(Integer&&)" << std::endl;
        pVal_ = other.pVal_;
        other.pVal_ = nullptr;
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

    // This allows expressions like:
    // Integer n1, n2, n3;
    // n2 = n1 + 1; // resolved as: n1.operator+(1)
    // n3 = n2 + n1; // resolved as: n2.operator+(n1)
    // but not:
    // n2 = 1 + n2;
    // If the first operand of the overloaded operator is of primitive type then
    // operator should be overloaded as global function.
    // Integer operator+ (const Integer& other) const {
    //     return Integer(this->GetValue() + other.GetValue());
    // }

    // pre-increment operator
    Integer& operator++() {
        ++(*(this->pVal_));
        return *this;
    }

    // post-increment operator
    // - needs to have an int argument so its signature differs from pre-increment operator
    // - needs to return the state of the object before the increment => we need to return
    // a temporary => return type can't be a reference
    // This is the reason why post-increment operator is less efficient than pre-increment.
    Integer operator++(int) {
        Integer n(*(this->pVal_));
        ++(*(this->pVal_));
        return n;
    }

    // comparison operator
    bool operator==(const Integer& other) const {
        return *(this->pVal_) == other.GetValue();
    }

    // Assignment Operator
    // If we don't implement custom assignment operator, compiler will provide its own
    // implementation which performs shallow copy. Shallow copy can have dangerous consequences if objects own
    // resources e.g. memory. To prevent shallow copying (e.g. just copying pointers but not copyingn objects
    // in memory) we need to provide our own implementation of assignment operator.
    Integer& operator= (const Integer& other){
        // check for self-assignment
        if (this != &other) {
            delete this->pVal_;
            this->pVal_ = new int(*(other.pVal_));
        }
        return *this;
    }

    // Move Assignment Operator
    Integer& operator= (Integer&& other){
        // check for self-assignment
        if (this != &other) {
            delete this->pVal_;
            this->pVal_ = other.pVal_;
            other.pVal_ = nullptr;
        }
        return *this;
    }

    // Function call operator
    // - makes this object a "function object"
    // - used extensively in STL
    // - can accept any number of arguments
    // - can be used with templates to implement callbacks
    void operator()() {
        std::cout << "Integer::operator()" << std::endl;
    }
};


Integer operator+(const Integer& n1, const Integer& n2){
    Integer n;
    n.SetValue(n1.GetValue() + n2.GetValue());
    return n;
}

Integer operator+(int n1, const Integer& n2){
    Integer n;
    n.SetValue(n1 + n2.GetValue());
    return n;
}

// https://docs.microsoft.com/en-us/cpp/standard-library/overloading-the-output-operator-for-your-own-classes?view=vs-2019
std::ostream& operator<< (std::ostream& os, const Integer& n){
    os << n.GetValue();
    return os;
}

std::istream& operator>> (std::istream& is, Integer& n) {
    int i;
    is >> i;
    n.SetValue(i);
    return is;
}

//
// Operator Overloading
//
//- allows custom implementation of primitive operators
//- allows usage of user-defined objects in mathematical expressions
//- operators are implemented as functions in form:
//     TRetVal operator#(TArg1 arg1, TArg2 arg2, ...)
//- operators can be defined as class/struct members and/or global functions
//- if class members, they always have 'this' passed to them as an implicit argument
//- compiler ensures that operator's basic behaviour does not change:
// associativity, precedence & arity (number of operands e.g. unary, binary etc.) do not change
//- operator functions should be non-static (except for new and delete)
//- operator that accepts more than one argument should always have one of the operands as user-deined type
//- if binary operator accepts a primitive type as the first argument then it has to be overloaded as global function
//- If the first operand is a user-defined type, then you can overload as member or global function.
//- overloading of operators . ?: sizeof etc...is not allowed
//- cannot define new operators or overload them
//- do operator overloads only to make operations conventional; otherwise they can be confusing
//
//

void operator_overloading_demo() {
    Integer n1(1), n2(2);

    // If operator+ is defined both as a member and as a global function, g++ issues a warning for the following expression.
    // warning: ISO C++ says that these are ambiguous, even though the worst conversion for the first is better than
    // the worst conversion for the second
    // g++ compiler uses global function
    auto n3 = n1 + n2;
    assert(n3.GetValue() == 3);
    std::cout << "n3.GetValue() = " << n3.GetValue() << std::endl;

    ++n3;
    assert(n3.GetValue() == 4);
    std::cout << "++n3; n3.GetValue() = " << n3.GetValue() << std::endl;

    n3++;
    assert(n3.GetValue() == 5);
    std::cout << "n3++; n3.GetValue() = " << n3.GetValue() << std::endl;

    std::cout << "(n3++).GetValue() = " << (n3++).GetValue() << std::endl; // 5
    assert(n3.GetValue() == 6);
    std::cout << "n3.GetValue() = " << n3.GetValue() << std::endl;

    Integer n4(6);
    assert(n3 == n4);

    Integer n5;
    n5 = n4;
    assert(n5.GetValue() == n4.GetValue());

    // this can be operator+ defined either as a member or globally
    Integer n6 = n5 + 1;
    assert(n6 == 7);

    // 1 gets converted to Integer via Integer(int) c-tor
    // Here is called
    //      Integer operator+(const Integer& n1, const Integer& n2)
    // unless
    //      Integer operator+(int n1, const Integer& n2)
    // is defined.
    Integer n7 = 1 + n6;
    assert(n7 == 8);

    // If we don't overload operator<< globally we'll get the following error:
    // error: no match for ‘operator<<’ (operand types are ‘std::basic_ostream<char>’ and ‘operators_demo::Integer’)
    std::cout << "n7 = " << n7 << std::endl;

    Integer n8;
    std::cout << "Enter the integer upon which will Integer custom type instance be created: " << std::endl;
    // If we don't overload operator>> globally we'll get the following error:
    // error: no match for ‘operator>>’ (operand types are ‘std::istream {aka std::basic_istream<char>’ and ‘operators_demo::Integer’)
    std::cin >> n8;
    std::cout << "n8 = " << n8 << std::endl;

    n8();

    Integer n9;
    // Compiler resolves the following expression as:
    // operator<<(cout, n9).operator<<(endl);
    // We have overloaded operator<< for Integer and it returns cout.
    // The operator << returns a cout object and that invokes the operator<< for endl operand, as
    // it is already overloaded in the ostream class.
    std::cout << n9 << std::endl;
}

void run() {
    std::cout << "operators_demo::run()" << std::endl;
    operator_overloading_demo();
}

}
