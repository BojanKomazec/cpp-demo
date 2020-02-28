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

namespace raw_pointers_demo {

// Idea: Project lasts as long as one person is working on it.
// If no employees are working on project, Project should be destroyed.
class Project {};

class Employee {
    Project* pProject_;
public:
    void set_project(Project* pProject) {
        pProject_ = pProject;
    }

    Project* get_project() const {
        return pProject_;
    }
};

// We can't use Employee2 as it won't compile
//
// class Employee2 {
//     std::unique_ptr<Project> pProject_;
// public:
//     void set_project(std::unique_ptr<Project> pProject) {
//         // error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>& std::unique_ptr<_Tp, _Dp>::operator=(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = smart_pointers_demo::Project; _Dp = std::default_delete<smart_pointers_demo::Project>]’
//         pProject_ = pProject;
//     }

//     std::unique_ptr<Project> get_project() const {
//         // error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = smart_pointers_demo::Project; _Dp = std::default_delete<smart_pointers_demo::Project>]’
//         return pProject_;
//     }
// };

// We can't use Employee3 as it won't compile
//
// class Employee3 {
//     std::unique_ptr<Project> pProject_;
// public:
//     void set_project(std::unique_ptr<Project>& pProject) {
//         // error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>& std::unique_ptr<_Tp, _Dp>::operator=(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = smart_pointers_demo::Project; _Dp = std::default_delete<smart_pointers_demo::Project>]’
//         pProject_ = pProject;
//     }

//     std::unique_ptr<Project> get_project() const {
//         // error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = smart_pointers_demo::Project; _Dp = std::default_delete<smart_pointers_demo::Project>]’
//         return pProject_;
//     }
// };

class ProjectManager {
    Project* pProject_;
public:
    ProjectManager() : pProject_(new Project()){}
    ~ProjectManager() { delete pProject_;}
    Project* get_project() const {return pProject_; }
};

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

void resource_shared_among_multiple_objects_demo_1() {
    Project* pProject = new Project();
    Employee employee1, employee2;
    employee1.set_project(pProject);
    employee2.set_project(pProject);
    // no employees are working on project => we can delete it
    delete pProject;
}

Project* CreateProject() {
    return new Project();
}

Employee* CreateEmployee(Project* pProject) {
    Employee* pEmployee = new Employee();
    pEmployee->set_project(pProject);
    return pEmployee;
}

void employees_project_engagement(const ProjectManager& pm) {
    Employee* employee1 = CreateEmployee(pm.get_project());
    Employee* employee2 = CreateEmployee(pm.get_project());
    delete employee1;
    delete employee2;
    // no employees are working on project => Project should be deleted
    // Who shall delete Project?
    // Is ProjectManager owning a project? Is it deleting it?
    // Should Employee try to delete Project?
    // Shall Project be deleted here?
    // It is not clear here what to do.
    // Ideally, Project pointer would be deleted automatically.
    // We can't wrap it in unique_ptr as we can't use unique_ptr to share underlying resource among multiple objects.
}

void resource_shared_among_multiple_objects_demo_2() {
    ProjectManager pm;
    employees_project_engagement(pm);
}

void resource_shared_among_multiple_objects_demo_3() {
    std::unique_ptr<Project> pProject{new Project};
    // Employee2* employee21 = new Employee2(); // won't work with Employee3 either
    // Employee2* employee22 = new Employee2();
    // // error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = smart_pointers_demo::Project; _Dp = std::default_delete<smart_pointers_demo::Project>]’
    // employee21->set_project(pProject);
    // employee22->set_project(pProject);

    //
    // We need to use some smart pointer which allows copying (shared ownership over underlying resource).
    // That is shared_ptr.
    //
}

void demo() {
    std::cout << "raw_pointers_demo()" << std::endl;

    operate(1);

    resource_shared_among_multiple_objects_demo_1();
    resource_shared_among_multiple_objects_demo_2();
    resource_shared_among_multiple_objects_demo_3();
}

} // namespace raw_pointers_demo

namespace unique_ptr_demo {

void PassUniquePtrByVal(std::unique_ptr<Integer> p) {
    std::cout << "PassUniquePtrByVal(): value = " << p->GetValue() << std::endl;
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
void demo() {
    std::cout << "unique_ptr::demo()" << std::endl;
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

} //namespace unique_ptr_demo

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

namespace shared_ptr_demo {

void PassSharedPtrByVal(std::shared_ptr<Integer> p, int n) {
    std::cout << "PassSharedPtrByVal(): setting new value = " << n << std::endl;
    p->SetValue(n);
    std::cout << "PassSharedPtrByVal(): value = " << p->GetValue() << std::endl;
}

class Project {};

class Employee {
    std::shared_ptr<Project> pProject_;
public:
    void set_project(std::shared_ptr<Project> pProject) {
        pProject_ = pProject;
    }

    std::shared_ptr<Project> get_project() const {
        return pProject_;
    }
};

void resource_shared_among_multiple_objects_demo() {
    std::cout << "resource_shared_among_multiple_objects_demo()" << std::endl;
    std::shared_ptr<Project> pProject{new Project{}};
    auto employee1 = new Employee{};
    auto employee2 = new Employee{};
    employee1->set_project(pProject);
    employee2->set_project(pProject);
    delete employee1;
    delete employee2;
}

std::shared_ptr<Project> create_project() {
    return std::shared_ptr<Project>(new Project());
}

std::shared_ptr<Employee> create_employee(std::shared_ptr<Project> pProject) {
    std::shared_ptr<Employee> employee(new Employee);
    employee->set_project(pProject);
    return employee;
}

void resource_shared_among_multiple_objects_demo2() {
    std::cout << "resource_shared_among_multiple_objects_demo2()" << std::endl;
    auto pProject = create_project();
    auto employee1 = create_employee(pProject);
    auto employee2 = create_employee(pProject);
}

// std::shared_ptr
// - used when you want to shre an underlying resource with other functions (other parts of code)
// - it keeps track how many of its copies are created
// - internally it uses a reference counting
// - each time a copy is created, it reference count increments by 1
// - when shared_ptr is destroyed, reference count is decremented
// - when reference count beecomes 0 then resource gets released/cleared
// - there is no deep copy taking place when copying instances of shared_ptr; all copies contain pointers with the same value
// - all the copies can see the current refernce count
void demo() {
    std::cout << "shared_ptr::demo()" << std::endl;

    std::shared_ptr<Integer> p0;
    // uninitialized shared_ptr has internal pointer set to nullptr
    assert(p0.get() == nullptr);
    // It is not possible to call methods on it. The following line would cause
    //      Segmentation fault (core dumped)
    // p0->SetValue(1);

    std::shared_ptr<Integer> p(new Integer);
    p->SetValue(1);
    assert((*p).GetValue() == 1);
    std::cout << "Integer value = " << p->GetValue() << std::endl;

    Integer* rawPointerToInteger = p.get();
    assert((*rawPointerToInteger).GetValue() == 1);

    p.reset(new Integer{3});

    // A copy of shared_ptr is created (copy c-tor is invoked)
    PassSharedPtrByVal(p, 2);

    // we can still use shared_ptr p after it has been passed to some other function
    std::cout << "Integer value = " << p->GetValue() << std::endl;
    p->SetValue(3);
    std::cout << "Integer value = " << p->GetValue() << std::endl;

    resource_shared_among_multiple_objects_demo();
    resource_shared_among_multiple_objects_demo2();
}

} // namespace shared_ptr_demo

namespace weak_ptr_demo_1 {

class Printer {
    int* pInt_;
public:
    void set_value(int* pInt) {
        pInt_ = pInt;
    }

    void print() const {
        std::cout << "Printer's int value = " << *pInt_ << std::endl;
    }
};

void show_the_problem() {
    std::cout << "weak_ptr::show_the_problem()" << std::endl;

    Printer printer;
    int n = 12;
    int* pInt = new int{n};
    printer.set_value(pInt);

    if (*pInt > 10) {
        delete pInt;
        // good practice: prevent crash if delete pInt is attempted again later
        pInt = nullptr;
    }

    // Problem: if pInt is deleted, printer's pointer points to invalid memory and when it's dereferenced, we get some random value:
    // output: Printer's int value = 1057813152
    printer.print();
    // Ideally, Printer.print() should be able to check if Printer.pInt_ is valid pointer or not.
    // The fact that we assigned nullptr to pInt does not help as (unlike deleting pInt) this change is not reflected onto Printer.pInt_.

    delete pInt;
}

class Printer1 {
    std::shared_ptr<int> pInt_;
public:
    void set_value(std::shared_ptr<int> pInt) {
        pInt_ = pInt;
    }

    void print() const {
        std::cout << "Printer1::print(): value = " << *pInt_ << std::endl;
        std::cout << "Printer1::print(): int* ref count = " << pInt_.use_count() << std::endl;
    }
};

// int* is shared between two parts of code so let's try to use shared_ptr
void solution_attempt(){
    std::cout << "weak_ptr::solution_attempt()" << std::endl;

    Printer1 printer;
    int n = 12;
    std::shared_ptr<int> pInt{new int{n}};
    // ref count is now 1

    printer.set_value(pInt);
    // ref count is now 2

    if (*pInt > 10) {
        pInt = nullptr; // or call reset()
        // ref count is now 1
        // Problem: underlying memory is not released yet but we actually do want it to be released here.
        // shared_ptr in Printer1 blocked releasing the memory as ref count is > 0.
    }

    printer.print();
    // output: Printer's int value = 12
    // We want shared_ptr somehow to unblock deleting underlying pointer when somewhone attempts to delete it (even if ref count is > 0).
}

//
// std::weak_ptr
//
// weak_ptr is related to shared_ptr
// shared_ptr contains pointer to the allocated memory and control block.
// Control block contains the reference count - how many copies of shared_ptr point to that same memory.
//      std::shared_ptr<int> sp{new int{1}};
// weak_ptr is always initialized with the shared_ptr:
//      std::weak_ptr<int> wp = sp;
// When a new weak_ptr points to shared_ptr, that does not increment shared_ptr's ref count of the shared_ptrs pointint to the resource.
// weak_ptr internally points to shared_ptr's control block; it has the information about the current reference count.
// If we deallocate memory by resetting the (single) shared_ptr:
//      sp.reset();
// wp will still be pointing to sp's control block
// Control block is destroyed when weak_ptr is destroyed.
// There can be multiple weak_ptrs pointing to the same shared_ptr (its control block).
// shared_ptr's control block maintains the reference count of associated weak_ptrs.
// control block gets destroyed when the weak_ptr reference count becomes zero.
// It is not possible to access underlying pointer in shared_ptr via weak_ptr.
// weak_ptr.expired() returns true if weak_ptr reference count is zero. That indicates that the shared_ptr and underlying
// memmory are not longer available.
// weak_ptr.lock() increases the reference count by 1 and returns shared_ptr.
// Other shared_ptr might be destroyed but ref count will not become 0.
//

class Printer2 {
    // weak_ptr is weakly pointing to the resource that may be available or not.
    // The idea is that we don't want this class (this pointer) to block external deletion of that resource.
    // The lifetime of the resource is managed externally.
    // This class can use resource as long as resource exists.
    std::weak_ptr<int> pInt_;
public:
    void set_value(std::weak_ptr<int> pInt) {
        pInt_ = pInt;
    }

    void print() const {
        std::cout << "Printer2::print(): weak_ptr ref count = " << pInt_.use_count() << std::endl;

        if (pInt_.expired()) {
            std::cout << "Printer2::print(): shared_ptr has released memory (resource not available anymore)." << std::endl;
            return;
        }

        auto sharedPtr = pInt_.lock();
        std::cout << "Printer2::print(): weak_ptr ref count = " << pInt_.use_count() << std::endl;
        std::cout << "Printer2::print(): shared_ptr ref count = " << sharedPtr.use_count() << std::endl;
        std::cout << "Printer2::print(): value = " << *sharedPtr << std::endl;
    }
};

void solution(){
    std::cout << "weak_ptr::solution()" << std::endl;

    Printer2 printer;
    int n = 12;
    std::shared_ptr<int> pInt{new int{n}};
    // ref count is now 1

    printer.set_value(pInt);
    // ref count is now 2

    if (*pInt > 10) {
        pInt = nullptr; // or call reset()
        // ref count is now 1
        // Problem: underlying memory is not released yet but we actually do want it to be released here.
        // shared_ptr in Printer1 blocked releasing the memory as ref count is > 0.
    }

    printer.print();
    // output: Printer's int value = 12
    // We want shared_ptr somehow to unblock deleting underlying pointer when somewhone attempts to delete it (even if ref count is > 0).
}


void demo() {
    std::cout << "weak_ptr::demo()" << std::endl;
    show_the_problem();
    solution_attempt();
    solution();
}

}

namespace weak_ptr_demo_2 {

// forward declaration
struct Employee;

//
// The following setup shows Circular Reference.
// Two objects have pointers to each other.
//

struct Project {
    std::shared_ptr<Employee> employee_;
    Project(){
        std::cout << "Project::Project()" << std::endl;
    }
    ~Project(){
        std::cout << "Project::~Project()" << std::endl;
    }
};

struct Employee {
    std::shared_ptr<Project> project_;
    Employee(){
        std::cout << "Employee::Employee()" << std::endl;
    }
    ~Employee(){
        std::cout << "Employee::~Employee()" << std::endl;
    }
};

void show_problem() {
    std::shared_ptr<Employee> employee{new Employee()};
    // Employee ref count = 1
    std::shared_ptr<Project> project{new Project()};
    // Project ref count = 1
    employee->project_ = project;
    // Project ref count = 2
    project->employee_ = employee;
    // Employee ref count = 2

    // when employee is destroyed => Employee ref count = 1 => underlyng memory for Employee is not released
    // when project is destroyed => Project ref count = 1 => underlyng memory for Project is not released

    // Output:
    //      Employee::Employee()
    //      Project::Project()
    // Problem: destructors are not called - memory is not released => memory leak!
    // When we use shared_ptrs in a Circular Reference setup, underlying memory will not be released.
}

struct Employee2;

struct Project2 {
    std::weak_ptr<Employee2> employee_;
    Project2(){
        std::cout << "Project2::Project2()" << std::endl;
    }
    ~Project2(){
        std::cout << "Project2::~Project2()" << std::endl;
    }
};

struct Employee2 {
    std::weak_ptr<Project2> project_;
    Employee2(){
        std::cout << "Employee2::Employee2()" << std::endl;
    }
    ~Employee2(){
        std::cout << "Employee2::~Employee2()" << std::endl;
    }
};

void show_solution() {
    std::shared_ptr<Employee2> employee{new Employee2()};
    // Employee2 ref count = 1
    std::shared_ptr<Project2> project{new Project2()};
    // Project2 ref count = 1
    employee->project_ = project;
    // Project2 ref count = 1
    project->employee_ = employee;
    // Employee2 ref count = 1

    // when employee is destroyed => Employee2 ref count = 0 => underlyng memory for Employee2 is released
    // when project is destroyed => Project2 ref count = 0 => underlyng memory for Project2 is released

    // Output:
    //      Employee2::Employee2()
    //      Project2::Project2()
    //      Project2::~Project2()
    //      Employee2::~Employee2()
}

void demo() {
    show_problem();
    show_solution();
}
}

// RAII (Resource Acquisition Is Initialization)
// The lifetime of the resource is bound to the local object so when object
// goes out of scope, its destructor will automatically release the resource.

void run() {
    std::cout << "smart_pointers_demo::run()" << std::endl;
    raw_pointers_demo::demo();
    memory_leak_demo();
    // segmentation_fault_demo();
    unique_ptr_demo::demo();
    shared_ptr_demo::demo();
    weak_ptr_demo_1::demo();
    weak_ptr_demo_2::demo();
}

}
