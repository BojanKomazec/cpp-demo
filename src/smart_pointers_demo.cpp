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

// std::unique_ptr
// - used when you don't want to share underlying resource
// - it does NOT support copy semantics (copy c-tor and assignment operator are deleted member functions) => we cannot create a copy of the unique_ptr
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
    memory_leak_demo();
    // segmentation_fault_demo();
    unique_ptr_demo();
    shared_ptr_demo();
}

}
