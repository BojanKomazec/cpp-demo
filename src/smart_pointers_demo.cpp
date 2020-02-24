#include <smart_pointers_demo.hpp>
#include <iostream>
#include <cassert>
#include <memory>

namespace smart_pointers_demo {


class Integer {
    int* pVal_ {nullptr};
public:
    Integer() {
        // std::cout << "Integer::Integer()" << std::endl;
        pVal_ = new int(0);
        std::cout << "Integer::Integer(). pVal_ = " << pVal_ << std::endl;
    }
    Integer(int n) {
        std::cout << "Integer::Integer(int)" << std::endl;
        pVal_ = new int(n);
    }
    ~Integer() {
        std::cout << "Integer::~Integer()" << std::endl;
        delete pVal_;
        pVal_ = nullptr;
    }

    Integer& operator=(const Integer& other) {
        std::cout << "Integer::operator=()" << std::endl;
        if (this != &other) {
            delete pVal_;
            pVal_ = new int{*other.pVal_};
        }
        return *this;
    }

    void SetValue(int n) {
         std::cout << "Integer::SetValue(int): n = " << n << std::endl;
        if (pVal_ != nullptr) {
            delete pVal_;
        }
        pVal_ = new int(n);
    }

    int GetValue() const {
        return *pVal_;
    }
};

class IntegerPtr{
    Integer *pInteger_ {nullptr};
public:
    IntegerPtr(Integer* pInteger):pInteger_(pInteger) {
        std::cout << "IntegerPtr::IntegerPtr()" << std::endl;
    }

    ~IntegerPtr() {
        std::cout << "IntegerPtr::~IntegerPtr()" << std::endl;
        delete pInteger_;
        pInteger_ = nullptr;
    }

    // overloaded arrow operator
    Integer* operator->() {
        return pInteger_;
    }

    // overloaded dereference operator
    Integer& operator*() {
        return *pInteger_;
    }
};

void memory_leak_demo() {
    std::cout << "memory_leak_demo()" << std::endl;
    {
        Integer *pi = new Integer;
        // Memory leak if we forget to call delete pi
        // delete pi;
        std::cout << "Memory leak!" << std::endl;
    }

    // Solution for the problem (forgetting to delete pointer):
    // Wrap resource in a local instance of RAII class:
    {
        std::cout << "IntegerPtr as RAII object:" << std::endl;
        // pInteger is local object; when it goes out of scope underlying pointer gets deleted automatically.
        IntegerPtr pInteger(new Integer);

        // calling overloaded arrow operator
        pInteger->SetValue(2);

        // calling overloaded dereference operator
        Integer& i = *pInteger;
        i.SetValue(3);
    }
}

void PassUniquePtrByVal(std::unique_ptr<Integer> p) {
    std::cout << "PassUniquePtrByVal(): value = " << p->GetValue() << std::endl;
}

void PassSharedPtrByVal(std::shared_ptr<Integer> p, int n) {
    std::cout << "PassSharedPtrByVal(): setting new value = " << n << std::endl;
    p->SetValue(n);
    std::cout << "PassSharedPtrByVal(): value = " << p->GetValue() << std::endl;
}

Integer* create_integer(int n) {
    return new Integer{n};
}

void display(Integer* pInteger) {
    if (pInteger) {
        std::cout << "display(): pInteger->GetValue() = " << pInteger->GetValue() << std::endl;
    } else {
        std::cout << "display(): pInteger is nullptr" << std::endl;
    }
}

// When this function is executed we can see that number of destructors matches the number or constructors
// which means there are no memory leaks. But it is very easy to forget deleting all pointers manually and
// so to introduce memory leaks. Modern C++ discourages writing code like this. Instead of using raw pointers
// C++11 encourages developers to use smart pointers which are using RAII concept to automatically delete
// pointers.
void operate(int n) {
    std::cout << "operate()" << std::endl;

    Integer* pInteger = create_integer(n);
    if (pInteger == nullptr) {
        pInteger = new Integer{n};
    }
    pInteger->SetValue(1);
    display(pInteger);
    delete pInteger;
    pInteger = nullptr;

    pInteger = new Integer{};
    // __LINE__ standard C macro that automatically expands into current line number
    *pInteger = __LINE__;
    display(pInteger);
    delete pInteger;
}

void raw_pointers_demo() {
    std::cout << "raw_pointers_demo()" << std::endl;

    operate(1);
}

void pass_unique_ptr_by_value(std::unique_ptr<Integer> pInteger) {
    std::cout << "pass_unique_ptr_by_value(). Value = " << pInteger->GetValue() << std::endl;
}

void pass_unique_ptr_by_ref(std::unique_ptr<Integer>& pInteger) {
    std::cout << "pass_unique_ptr_by_ref(). Value = " << pInteger->GetValue() << std::endl;
}

// operate() function rewritten to use unique_ptr.
// It is simpler and safer than operate(). We don't need to deal with memory management (call delete explicitly).
void operate_with_unique_ptr(int n) {
    std::cout << "operate_with_unique_ptr()" << std::endl;

    std::unique_ptr<Integer> pInteger{create_integer(n)};
    if (pInteger == nullptr) {
        // error: no match for ‘operator=’ (operand types are ‘std::unique_ptr<smart_pointers_demo::Integer>’ and ‘smart_pointers_demo::Integer*’)
        // pInteger = new Integer{n};

        // If the smart pointer holds an existing pointer, it will be deleted first and then it will take ownership of the new pointer
        pInteger.reset(new Integer{n});
    }

    // We can call the member functions of the underlying pointer.
    // Arrow operator is overloaded:
    pInteger->SetValue(1);

    // direct access to underlying raw pointer is needed
    display(pInteger.get());

    // Overload of operator= which accepts nullptr as a parameter.
    // It deletes underlying pointer and assigns nullptr to it.
    pInteger = nullptr;

    pInteger.reset(new Integer{});

    // We can call the member functions of the underlying pointer.
    // Dereferencing operator is overloaded:
    *pInteger = __LINE__;

    display(pInteger.get());

    // copy c-tor is deleted => can't copy unique_ptr
    // error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = smart_pointers_demo::Integer; _Dp = std::default_delete<smart_pointers_demo::Integer>]’
    // std::unique_ptr<Integer> pInteger2(pInteger);
    // unique_ptr has move constructor: unique_ptr::unique_ptr(unique_ptr&& __u)
    std::unique_ptr<Integer> pInteger2(std::move(pInteger));

    // Don't try to access underlying pointer in pInteger
    // Nevertheless, we can reuse it and assign a new pointer to it:
    pInteger.reset(new Integer{2});

    // copy c-tor is deleted => can't copy unique_ptr
    // error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = smart_pointers_demo::Integer; _Dp = std::default_delete<smart_pointers_demo::Integer>]’
    // pass_unique_ptr_by_value(pInteger2);

    // We don't use pInteger2 after pass_unique_ptr_by_value(), pInteger2 will be destroyed =>
    // we can move underlying resource into the new unique_ptr
    pass_unique_ptr_by_value(std::move(pInteger2));

    std::unique_ptr<Integer> pInteger3;
    // assignment operator is deleted => we can't assign one unique_ptr to another
    // error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>& std::unique_ptr<_Tp, _Dp>::operator=(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = smart_pointers_demo::Integer; _Dp = std::default_delete<smart_pointers_demo::Integer>]’
    // pInteger3 = pInteger;
    // unique_ptr has move assignment operator: unique_ptr::operator=(unique_ptr&& __u)
    pInteger3 = std::move(pInteger);

    // Passing by ref is useful when we still want to use uniqie_ptr after we pass it to some function.
    pass_unique_ptr_by_ref(pInteger3);

    std::cout << "pInteger3 value = " << pInteger3->GetValue() << std::endl;
}

// std::unique_ptr
// - used when underlying resource (pointer) doesn't have to be shared with other parts of the code
// - it is a class template so we have to specify the type (not pointer to type) in angular brackets
// - it does NOT support copy semantics (copy c-tor and assignment operator are deleted member functions) =>
//      we cannot create a copy of the unique_ptr
// - it has an explicit c-tor => we cannot use assignment to initialize it; we need to use direct initialization
// - it supports move semantics (move c-tor and assignment operators are defined) => we can move the resource ownership
// - after it's been moved, this pointer should not be used
void unique_ptr_demo() {
    std::cout << "unique_ptr_demo()" << std::endl;
    std::unique_ptr<Integer> p(new Integer);
    p->SetValue(1);
    std::cout << "Integer value = " << p->GetValue() << std::endl;

    // unique_ptr's copy constructor is deleted member function.
    // error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = smart_pointers_demo::Integer; _Dp = std::default_delete<smart_pointers_demo::Integer>]’
    // std::unique_ptr<Integer> p2(p);
    // std::unique_ptr<Integer> p2 = p;

    std::unique_ptr<Integer> p2;

    // unique_ptr's assignment operator is deleted member function.
    // error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>& std::unique_ptr<_Tp, _Dp>::operator=(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = smart_pointers_demo::Integer; _Dp = std::default_delete<smart_pointers_demo::Integer>]’
    // p2 = p;

    // Passing an object by value means creating its copy and calling copy-ctor but this one is deleted:
    // error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = smart_pointers_demo::Integer; _Dp = std::default_delete<smart_pointers_demo::Integer>]’
    // PassUniquePtrByVal(p);

    // we can enforce calling move c-tor which will move resource ownership from p to the argument passed into function
    PassUniquePtrByVal(std::move(p));

    // After it was moved p does not own resourse and should not be used.
    //
    // Using of p after it was moved generates runtime error:
    //  Segmentation fault (core dumped)
    // std::cout << "Value held in unique_ptr after it was moved: " << p->GetValue() << std::endl;
    //
    // Resource held by passed pointer will be destroyed/cleared when PassUniquePtrByVal() returns and
    // we won't be able to use that resource after.
    //
    // If we want to share ownership of the resource with PassUniquePtrByVal() (so we can keep
    // accessing resource after PassUniquePtrByVal() returns) we need to use std::shared_ptr

    operate_with_unique_ptr(1);
}

void segmentation_fault_demo() {
    // The following code causes segmentation fault (uncomment to check it out):
    // {
    //     IntegerPtr pInteger(new Integer);

    //     // A new Integer object is created from the reference to the existing Integer instance
    //     // by calling DEFAULT copy c-tor
    //     Integer i = *pInteger;

    //     std::cout << i.GetValue() << std::endl;

    //     // Here first local Integer object gets destructed thus deleting memory.
    //     // Then IntegerPtr gets destructed so ~IntegerPtr calls ~Integer which then again tries to delete memory.
    // }

    // The following works ok:
    {
        IntegerPtr pInteger(new Integer);
        Integer& i = *pInteger;
        std::cout << i.GetValue() << std::endl;
        // When reference to object goes out of scope it does not call object's destructor.
        // Reference is just a reference, an alias.
        // If object is destructed, a reference to it should not be used.
        // https://stackoverflow.com/questions/8957524/in-c-if-i-reference-an-object-with-a-deallocating-destructor-myclass-will-th
        // Here pInteger goes out of scope, ~IntegerPtr calls ~Integer which deallocates memory for int.
        // Memory is deallocated once, as should be.
    }

     // The following works ok:
    {
        IntegerPtr pInteger(new Integer);
        // *pInteger returns a reference
        std::cout << (*pInteger).GetValue() << std::endl;
    }

    std::cout << "After the block scope" << std::endl;

    // new Integer;
    // std::unique_ptr<Integer> p(new Integer);

    std::shared_ptr<Integer> p(new Integer);
}

// std::shared_ptr
// - used when you want to shre an underlying resource with other functions
// - internally it uses a reference counting
// - each time a copy is created, it reference count increments by 1
// - when shared_ptr is destroyed, reference count is decremented
// - when reference count beecomes 0 then resource gets released/cleared
void shared_ptr_demo() {
    std::cout << "shared_ptr_demo()" << std::endl;
    std::shared_ptr<Integer> p(new Integer);
    p->SetValue(1);
    std::cout << "Integer value = " << p->GetValue() << std::endl;

    // A copy of shared_ptr is created (copy c-tor is invoked)
    PassSharedPtrByVal(p, 2);

    // we can still use shared_ptr p after it has been passed to some other function
    std::cout << "Integer value = " << p->GetValue() << std::endl;
    p->SetValue(3);
    std::cout << "Integer value = " << p->GetValue() << std::endl;
}

// RAII (Resource Acquisition Is Initialization)
// The lifetime of the resource is bound to the local object so when object
// goes out of scope, its destructor will automatically release the resource.

void run() {
    std::cout << "smart_pointers_demo::run()" << std::endl;
    raw_pointers_demo();
    memory_leak_demo();
    // segmentation_fault_demo();
    unique_ptr_demo();
    shared_ptr_demo();
}

}
