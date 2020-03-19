#include <class_demo.hpp>
#include <iostream>
#include <cassert>
#include <typeinfo>

namespace class_demo {

class Car {
    // non-static data member initializers (C++11)
    // Compiler injects this initialization code into all constructors.
    // Explicit data member initialization in c-tor takes precedence.
    float fuel_ {0};
    float speed_ {0};
    int passengers_ {0};

    // error: flexible array member ‘class_demo::Car::yearsMajorService_’ not at end of ‘class class_demo::Car’
    // error: initializer for flexible array member ‘int class_demo::Car::yearsMajorService_ []’
    // int yearsMajorService_[]{1997, 1999, 2001};
    // int yearsMajorService_[] = {1997, 1999, 2001};
    int yearsMajorService_[3]{1997, 1999, 2001};
    // or
    // int yearsMajorService_[3] = {1997, 1999, 2001};

    float someUnitializedFloat_; // this will initially be some random value

    int* p_ {}; // initializer initializes pointer to nullptr
    int* p2_ = new int(55);
    int* p3_ {new int(56)};

    // error: non-static data member declared with placeholder ‘auto’
    // auto autoMember_ = 1;

    int calculateInt(int n) { return n + 2; }
    int int1_ = calculateInt(3); // initialized to 5

    int n {234};

    // static data member
    // Not part of the objec but part of the class.
    // Declared inside the class but defined & initialized out of class.
    static int totalCarsCount;
public:

    // Constructor.
    // Can have arguments and be overloaded.
    // Default c-tor is the one with no arguments.
    // Compiler creates default c-tor if there are no other c-tors defined.
    Car() {
        std::cout << "Car::Car()" << std::endl;
        // By using non-static data member initializers we're preventing duplicating
        // in all constructors the following initialization code:
        // fuel_ = 0;
        // speed_ = 0;
        // passengers_ = 0;
        ++totalCarsCount;
    }

    Car(float amount) {
        std::cout << "Car::Car(float)" << std::endl;
        // Explicit data member initialization in c-tor takes precedence.
        fuel_ = amount;
        // By using non-static data member initializers we're preventing duplicating
        // in all constructors the following initialization code:
        // speed_ = 0;
        // passengers_ = 0;
        ++totalCarsCount;
    }

    // Copy constructor
    // Creates an object by copying the state of another object.
    // If user does not provide it, compiler will generate its default implementation which
    // only copies all members (shallow copies).
    // Object is copied if we:
    // - pass it by value to a function
    // - return an object by value from a function
    // - manually create object copy
    //
    // Custom copy c-tor should perform deep copy.
    // We pass other Car object by reference as if it was passed by value, then it would
    // cause recursive calling of this same copy c-tor.
    // Const reference is used to enforce not changing other object.
    Car(const Car& other) {
        // nullptr check is necessary as dereferencing nullptr will cause segmentation error in runtime.
        if (other.p_ != nullptr) {
            p_ = new int(*other.p_);
        }

        if (other.p2_ != nullptr) {
            p2_ = new int(*other.p2_);
        }

        if (other.p3_ != nullptr) {
            p3_  = new int(*other.p3_);
        }
    }

    // Rule of 3
    // If any of these class methods is implemented, then very likely the other two also have to be implemented:
    //  - destructor
    //  - copy constructor
    //  - copy assignment operator
    // Not implementing them all might cause memory leak or shallow copy (and issues it creates).

    // copy assignment operator
    // Car& operator=(const Car& other) {

    // }

    // Destructor is used to free resources when object is destroyed.
    // Has no arguments, can't be overloaded.
    // automatically called when object is deleted from heap or goes out from scope on stack.
    ~Car() {
        std::cout << "Car::~Car()" << std::endl;
        --totalCarsCount;

        if (p_ != nullptr) {
            delete p_;
            p_ = nullptr;
        }

        if (p2_ != nullptr) {
            delete p2_;
            p2_ = nullptr;
        }

        if (p3_ != nullptr) {
            delete p3_;
            p3_ = nullptr;
        }
    }

    void FillFuel(float amount);
    void Accelerate();
    void Brake();
    void AddPassengers(int count);
    void Dashboard() const;

    // Compiler injects the address of the object (pointer to the object) on which the member function is called
    // as the hidden parameter of the member function.
    void TestThisPointer(int n) {
        std::cout << "TestThisPointer(): n = " << n << std::endl;
        this->int1_ += 10;

        // this can't be assigned some other value; it's a const pointer
        // error: lvalue required as left operand of assignment
        // this = new Car();

        // name of the input arg shadows the name of the data member
        n = n; // this assigns input parameter its own value
        std::cout << "TestThisPointer(): n = " << n << std::endl;
        std::cout << "TestThisPointer(): this->n = " << this->n << std::endl;

        // we need to use this in order to access class member of the same name as the input argument
        this->n = n;
        std::cout << "TestThisPointer(): this->n = " << this->n << std::endl;

        // this can be dereferenced as any other pointer
        const Car& car = *this;
        // auto n = car.int1_
    }

    // static member function
    // belongs to class, not objects =>
    // does not receive this pointer so can't access non-static members

    // static member function can't be declared as 'const' as 'const' is used to show that method does not
    // change the state of the object but static methods by default don't interfere with objects.
    // error: static member function ‘static int class_demo::Car::GetTotalCarsCount()’ cannot have cv-qualifier
    // static int GetTotalCarsCount() const {
    static int GetTotalCarsCount() {
        return Car::totalCarsCount;
    }
};

// Static member definition (memory allocation). It cannot be in class methods.
// It is accessed via class name scope.
int Car::totalCarsCount; // 0 by default

void Car::FillFuel(float amount) {
    fuel_ = amount;
}

void Car::Accelerate(){
    speed_++;
    fuel_ -= 0.5f;
}

void Car::AddPassengers(int count) {
    passengers_ += count;
}

void Car::Brake(){
    speed_ = 0;
}

// If class members are not initialized, they will by default have random values (not default values for their types).
// E.g.
// Fuel = 3.08075e-41
// Speed = 1.97402e-08
// Passengers = 32764
// This method does not change the state of the class so it's declared as 'const'.
// 'const' is actually applied to *this.
void Car::Dashboard() const {
    std::cout << "fuel_ = " << fuel_ << "\n";
    std::cout << "speed_ = " << speed_ << "\n";
    std::cout << "passengers_ = " << passengers_ << "\n";
    std::cout << "someUnitializedFloat = " << someUnitializedFloat_ << "\n";
    std::cout << "p_ = " << p_ << "\n";
    std::cout << "*p2_ = " << *p2_ << "\n";
    std::cout << "*p3_ = " << *p3_ << "\n";
    std::cout << "int1_ = " << int1_ << "\n";
    std::cout << "Car::totalCarsCount = " << Car::totalCarsCount << "\n";
    std::cout << std::endl;
}

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

void class_demo() {
    Car car;
    car.Dashboard();
    car.TestThisPointer(789);
    car.FillFuel(6);
    car.Dashboard();
    car.Accelerate();
    car.Accelerate();
    car.Accelerate();
    car.Accelerate();
    car.Dashboard();
    car.Brake();
    car.Dashboard();

    // const object
    const Car car2;
    // Compiler does not allow calling non-const methods on const objects.
    // error: passing ‘const class_demo::Car’ as ‘this’ argument discards qualifiers [-fpermissive]
    // car2.FillFuel(6);

    // It's ok to call const member functions.
    car2.Dashboard();

    Car car3;

    std::cout << "Total cars count = " << Car::GetTotalCarsCount() << std::endl;
}

void copy_constructor_demo() {
    std::cout << "copy_constructor_demo()" << std::endl;
    Car car1;
    car1.Dashboard();

    {
        Car car2(car1); // copy c-tor is called
        car2.Dashboard();
        // car2 gets destroyed here thus deleteing copied pointers
    }

    // This is the conseqience of shallow copy of data members (pointers in this case):
    // car2 destructor delted all pointer members also for car1 so
    // they now show some random values:
    // p_ = 0
    // *p2_ = -1707244832
    // *p3_ = -1707244896
    car1.Dashboard();
}

// all struct members are always public
struct Point {
    int x;
    int y;
};

void struct_demo() {
    Point p;
    p.x = 12;
    p.y = 13;
    std::cout << "Point coordinates: (" << p.x << ", " << p.y << ")" << std::endl;
}

void copy_assignment_operator_demo() {
    std::cout << "copy_assignment_operator_demo()" << std::endl;

    Car car1;
    Car car2 = car1;
}

// Observe duplicated initialization code in constructors.
// This duplicated code might be made of multiple lines and with some complex expressions
// so duplicating it can introduce bugs.
class Car2 {
    float fuel_;
    float speed_;
    int passengers_;
    static int totalCarsCount;
public:
    Car2(){
        passengers_ = 0;

        // common initialization code in 2 constructors
        fuel_ = 0;

        // common initialization code in all constructors
        speed_ = 0;
        totalCarsCount++;
    }

    Car2(int passengers) {
        passengers_ = passengers;

        // common initialization code in 2 constructors
        fuel_ = 0;

        // common initialization code in all constructors
        speed_ = 0;
        totalCarsCount++;
    }

    Car2(float fuel, int passengers){
        fuel_ = fuel;
        passengers_ = passengers_;

        // common initialization code in all constructors
        speed_ = 0;
        totalCarsCount++;
    }
};

// class uses Constructor Delegation (C++11)
// Delegating constructors allows one c-tor to call another one.
// This prevents code duplication.
class Car3 {
    float fuel_;
    float speed_;
    int passengers_;
    static int totalCarsCount;
public:
    // Constructor Delegation
    Car3(): Car3(0) {
        std::cout << "Car3::Car3()" << std::endl;
    }

    // Constructor Delegation
    Car3(int passengers): Car3(0, passengers) {
        std::cout << "Car3::Car3(int)" << std::endl;
    }

    // Only one constructor contains initialization code.
    // Other constructors are calling this one in initializer list.
    // They must not call it in their bodies otherwise a new (separate) object would be created.
    // We could have put this common code in some other member function e.g. init() but this would
    // also be prone to errors as that function could be called at any time, not just during object
    // construction.
    Car3(float fuel, int passengers){
        std::cout << "Car3::Car3(float, int)" << std::endl;
        fuel_ = fuel;
        passengers_ = passengers_;
        speed_ = 0;
        totalCarsCount++;
    }
};

int Car3::totalCarsCount = 0;

void delegating_constructors_demo() {
    std::cout << "delegating_constructors_demo()" << std::endl;

    std::cout << "Creating Car3 with default c-tor" << std::endl;
    // Output shows the cascade of c-tors called:
    //  Car3::Car3(float, int)
    //  Car3::Car3(int)
    //  Car3::Car3()
    Car3 car31;

    std::cout << "Creating Car3 with c-tor with a single argument" << std::endl;
    Car3 car32(1);

    std::cout << "Creating Car3 with c-tor with two arguments" << std::endl;
    Car3 car33(10.0f, 1);
}

class IntegerA {
    int val_;
public:
    IntegerA() {
        val_ = 0;
    }

    IntegerA(int n) {
        val_ = n;
    }
};

class IntegerB {
    int val_ {};
public:
    // if we specify parameterized c-tor, compiler will not auto-generate the default one
    IntegerB(int n) {
        val_ = n;
    }
};

class IntegerC {
    int val_ {};
public:
    // if we specify parameterized c-tor, compiler will not auto-generate the default one
    IntegerC(int n) {
        val_ = n;
    }

    // From C++11 we can instruct compiler explicitly to create default c-tor.
    // 'default' can be used only with those functions that can be auto-generated by the compiler:
    // c-tor, copy c-tor, assignment operator and destructor.
    // default keyword works only on those functions that can be implicitly generated.
    // Parameterized constructors can never be generated by the compiler.
    IntegerC() = default;

    // we can use 'default' here although compiler would created this copy c-tor anyway
    IntegerC(const IntegerC&) = default;
};

class IntegerD {
    int val_ {};
public:
    IntegerD(int n) {
        val_ = n;
    }

    IntegerD() = default;

    // if we want to prevent making copies of this object, we need to tell compiler not to
    // create default copy c-tor. From C++11 we can use keyword 'delete' for this:
    IntegerD(const IntegerD&) = delete;

    void SetValue(int n) {
        val_ = n;
    }
};

class IntegerE {
    int val_ {};
public:
    IntegerE(int n) {
        val_ = n;
    }

    IntegerE() = default;

    // if we want to prevent making copies of this object, we need to tell compiler not to
    // create default copy c-tor. From C++11 we can use keyword 'delete' for this:
    IntegerE(const IntegerD&) = delete;

    // To make class non-copyable, we also need to prevent creation of copy assignment
    IntegerE& operator=(const IntegerE&) = delete;

    void SetValue(int n) {
        val_ = n;
    }

    // 'delete' can be used on ANY member function.
    // It prevents compiler allowing using SetValue with float argument.
    void SetValue(float) =  delete;
};

void default_and_deleted_member_functions_demo() {
    IntegerA i1; // default c-tor is called
    IntegerA i2(1); // parameterized c-tor is called

    // class has no default c-tor =>
    //  error: no matching function for call to ‘class_demo::IntegerB::IntegerB()’
    // IntegerB ib1;

    IntegerB ib2(1);

    IntegerC ic1;
    IntegerC ic2(1);
    IntegerC ic3(ic2);

    IntegerD id1;

    // error: use of deleted function ‘class_demo::IntegerD::IntegerD(const class_demo::IntegerD&)’
    // IntegerD id2(id1);

    id1.SetValue(2);
    id1.SetValue(3.14f); // ok

    IntegerE ie1;
    // error: use of deleted function ‘void class_demo::IntegerE::SetValue(float)’
    // ie1.SetValue(3.14f);
}

class IntegerF {
    int* pVal_;
public:
    IntegerF(int n) {
        pVal_ = new int(n);
    }
    ~IntegerF() {
        delete pVal_;
        pVal_ = nullptr;
    }

    //
    // friend keyword
    //
    // this function can access class' private members
    friend void GetValue(const IntegerF& n);
    // this class can access class' private members
    friend class Printer;
    // NOTE: using friendships is discouraged as it breaks the encapsulation and should only be used as the last resort
    // for solving an issue that requires access to private members.
};

// This is a global function that wants to access class's private member.
void GetValue(const IntegerF& n) {
    // If we don't declare this function a friend of class IntegerF then the following compilationerror will occur:
    //  error: ‘int* class_demo::IntegerF::pVal_’ is private within this context
    int val = *n.pVal_;
    std::cout << "IntegerF value = " << val << std::endl;
}

class Printer {
public:
    void Print(const IntegerF& n) {
        int val = *n.pVal_;
        std::cout << "Printer::Print(): IntegerF value = " << val << std::endl;
    }
};

void friend_demo() {
    IntegerF n(1);
    GetValue(n);

    Printer printer;
    printer.Print(n);
}

namespace initialization_vs_assignment{

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

    // pre-increment operator
    Integer& operator++() {
        ++(*(this->pVal_));
        return *this;
    }

    // post-increment operator
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
    Integer& operator= (const Integer& other){
        std::cout << "Integer::operator=(const Integer&)" << std::endl;
        if (this != &other) {
            delete this->pVal_;
            this->pVal_ = new int(*(other.pVal_));
        }
        return *this;
    }

    // Move Assignment Operator
    Integer& operator= (Integer&& other){
        std::cout << "Integer::operator=(Integer&&)" << std::endl;
        if (this != &other) {
            delete this->pVal_;
            this->pVal_ = other.pVal_;
            other.pVal_ = nullptr;
        }
        return *this;
    }

    void operator()() {
        std::cout << "Integer::operator()" << std::endl;
    }
};

class EntityA {
    Integer id_;
public:
    EntityA(const Integer& id) {
        std::cout << "EntityA::EntityA(const Integer&)" << std::endl;
        id_ = id;
    }
    ~EntityA() {
        std::cout << "EntityA::~EntityA()" << std::endl;
    }
};

class EntityB {
    Integer id_;
    int n_;
public:
    // Use member initializer list to initialize members in order to make members
    // created & initialized with a single function call (copy c-tor) rather then with
    // two function calls (default c-tor and assignment operator).
    // Class members are initialized in the initializer list in the order they are declared in class.
    EntityB(const Integer& id):id_(id), n_(id.GetValue()) {
        std::cout << "EntityB::EntityB(const Integer&)" << std::endl;
    }
    ~EntityB() {
        std::cout << "EntityB::~EntityB()" << std::endl;
    }
};

void demo(){
    std::cout << "initialization_vs_assignment::demo()" << std::endl;
    // initialization
    // parameterized c-tor is called; NO temp objects are being created => it is preferred to assignment
    Integer n1(1);

    // assignment
    // temp Integer object is built from 11, move assignment operator is called and then temp object is destroyed
    n1 = 11;

    std::cout << "n1 = " << n1.GetValue() << std::endl;

    // default c-tor is called
    Integer n2;

    // assignment
    // temp Integer object is built from 2, move assignment operator is called and then temp object is destroyed
    n2 = Integer(2);

    std::cout << "n2 = " << n2.GetValue() << std::endl;

    // temp Integer object is created from 1
    // EntityA parametrized c-tor is called and in it:
    // - EntityA::id_ is created with Integer default c-tor
    // - Integer assignment operator is called (assignment is used to initialize id_. We want to avoid this!)
    // temp Integer is destroyed
    EntityA ea(1);

    std::cout << "EntityA created." << std::endl;

    // temp Integer is created from 2
    // EntityB parametrized c-tor is called and in it:
    // - EntityB::id_ is created with copy c-tor (object is created & initialized with a single function call. This is what we want.)
    // temp Integer is destroyed
    EntityB eb(2);

    std::cout << "EntityB created." << std::endl;
}

}

//
// OOP
//
// System is made up of objects.
// Object is instance of a class.
// Classes and objects are related.
// Collaboration between objects defines the behaviour of the system.
// Relationship types: composition, inheritance
//
// Composition
//
// This is a relation between objects (not classes).
// Object composed in another object.
// Represents "has a" relation.
// Owner object reuses functionality of the part.
// e.g. Car has an engine
//
// Inheritance
//
// This is a relationship between classes.
// Allows (child) class to inherit and reuse features of another (parent) class.
// "is a" relationship.
// E.g. Animal class has behaviour eat(), sleep(), run() and Dog and Car inherit from Animal class
// Syntax: class Child : <optional_access_modifier> Parent
//
// Access Modifiers
//
// private - accessible only from withing that class
// protected - acessible also from a child class
// public - accessible by any other place
//
// Accessibility depends on the type of inheritance.
//
// For public inheritance: (parent -> child)
// private -> private (not accessible in child class)
// public -> public
// protected -> protected
//
// For private inheritance: (parent -> child)
// private -> private (not accessible in child class)
// public -> private (accessible in child class but not outside it)
// protected -> private (accessible in child class but not outside it)
//
// For protected inheritance: (parent -> child)
// private -> private (not accessible in child class)
// public -> protected (accessible in child class and its children)
// protected -> protected (accessible in child class and its children)
//
// If inheritance access modifier is not specified the default one is used and it depends on the child type. If child is:
// struct => public inheritance
// class => private inheritance
//
// Object construction
//
// Constructors execute from parent to child.
// Destructors executed from child to parent.
// Base data members will be part of child object.
//
//
// In C++ there are 3 member functions that are not inherited:
// - constructors
// - destructors
// - assignment operator
//
// Inheriting Constructors (C++11)
// - child class can inherit parent's constructor so we don't need manually to implement child's constructor.
// - compiler implicitly implements child class c-tors which call the base class c-tors.
// - This prevents repeating the same code - e.g. the one which assigns all c-tor args to data members.
//   Also this prevents compiler forcing us to implement child class c-tor which only calls base class c-tor.
// - This can't be used if child class has its own data member which has to be initialized in c-tor (but can be used if
//   we can initialize it outside the constructor).
//
//
// Polymorphism
//
// - Different forms of the function are provided.
// - The call is resolved at compile time or runtime.
// - Compile time polymorphism:
//      - Compiler has enough info in compile time to decide which function shall be called.
//      - Examples: function overloading, operator overloading, templates.
// - Runtime polymorphism (dynamic binding):
//      - Function to be invoked is decided in runtime
//      - Implemented through virtual mechanism.
//      - Compiler inserts code to invoke the correct function at runtime.
//      - Automatically generated through the 'virtual' keyword when declaring functions.
//      - Such functions are called polymorphic functions.
//      - Shold be invoked only through pointer or reference.
//
//
//  Virtual mechanism (Vtable and Vptr)
//
// - When compiler compiles class functions, it creates an array of functions pointers - a virutal table.
//   It contains only pointers to virtual functions.
// - Non-virtual functions don't appear in Vtable. They are always called on the class where thery are defined.
//   E.g. if fn1, fn2 and fn3 are virtual in Base class we'll have:
//      &Base::fn1
//      &Base::fn2
//      &Base::fn3
// - Starting address of this array (Vtable) is stored in a special class member variable called virtual pointer, Vptr.
// - Virtual pointer is a member variable of a class and is stored with other attributes in the object.
// - Compiler automatically adds Vptr to base class which has virtual function members. This increaases the size of the
//   class by the size of the pointer (4 bytes on 32-bit machines, 8 bytes on 64-bit machines).
// - Compiler automatically adds to its child classes both Vtable and Vptr.
// - Their Vptrs are pointing to their own Vtables.
// - Their Vtable is updated so it contains correct function addresses. If that function is overriden in this class, that
//   address will be set. If not, that would be the address of the function in the first parent class where this function
//   is overridden.
// - E.g. if Child class overrides fn1 and fn3 but not fn2:
//      &Child::fn1
//      &Base::fn2
//      &Child::fn3
// - What happens if we call virtual method on a child class?:
//      Base* p = &child; p->fn1();
// 1) Get the object address (&child)
// 2) Get the Vptr
// 3) Find the position of the function (fn1) in Vtable
// 4) Get the address of a function
// 5) Invoke the function
//
// - If class has not virtual member functions, that means that its author didn't make any provision for its behavior to be customized.
namespace oop_demo {

class Animal {
public:
    void eat() {
        std::cout << "Animal::eat()" << std::endl;
    }
    void run() {
        std::cout << "Animal::run()" << std::endl;
    }
    void speak() {
        std::cout << "Animal::speak()" << std::endl;
    }
};

class Dog : public Animal {

};

// Dog actually has some specific behaviour.
// When the child class reimplements function with the same name from the base class, these implementations will hide base implementation.
class Dog2 : public Animal {
public:
    void eat() {
        std::cout << "Dog2::eat()" << std::endl;
    }
    void speak() {
        std::cout << "Dog2::speak()" << std::endl;
    }
};

void demo() {
    Dog dog;
    dog.eat();
    dog.run();
    dog.speak();

    Dog2 dog2;
    dog2.eat();
    dog2.run();
    dog2.speak();
}

class Account {
    std::string name_;
    int id_;
    // error: ISO C++ forbids in-class initialization of non-const static member ‘class_demo::oop_demo::Account::id_generator_’
    // static int id_generator_ = 0;
    static int id_generator_;
protected:
    float balance_{0.0f};
public:
    Account(const std::string& name, float balance):name_(name), balance_(balance){
        id_ = ++Account::id_generator_;
        std::cout << "Account::Account()" << std::endl;
    }

    virtual ~Account(){
        std::cout << "Account::~Account()" << std::endl;
    }

    std::string get_name() const {
        return name_;
    }

    float get_balance() const {
        return balance_;
    }

    int get_id() const {
        return id_;
    }

    // Functions that are meant to be overriden in child classes are declared as virtual:

    virtual float get_interest_rate() const {
        return 0.0f;
    }

    virtual void accumulate_interest(){
    }

    virtual void withdraw(float amount){
        if (amount < balance_) {
            balance_ -= amount;
        } else {
            std::cout << "Insufficient balance." << std::endl;
        }
    }

    virtual void deposit(float amount){
        balance_ += amount;
    }
};

// error: ‘static’ may not be used when defining (as opposed to declaring) a static data member [-fpermissive]
// static int Account::id_generator_ = 0;
int Account::id_generator_ = 0;

class Savings : public Account {
    float rate_;
public:
    // If we don't call base class c-tor explicitly, compiler will do it by calling its default c-tor (which in our case does not exist):
    // error: no matching function for call to ‘class_demo::oop_demo::Account::Account()’
    // Savings(const std::string& name, float balance, float rate):rate_(rate){}

    Savings(const std::string& name, float balance, float rate) : Account(name, balance), rate_(rate){
        std::cout << "Savings::Savings()" << std::endl;
    }

    ~Savings(){
        std::cout << "Savings::~Savings()" << std::endl;
    }

    void accumulate_interest() override {
        balance_ += (rate_ * balance_);
    }

    float get_interest_rate() const override {
        return rate_;
    }
};

class Checking : public Account {
    const static int minimum_balance = 50;
public:
    // Problem: Compiler forces us to implement child class c-tor just to call base class c-tor.
    // This c-tor is valid but we'll use C++11 Inheriting Constructor
    // Checking(const std::string& name, float balance) : Account(name, balance_){}
    // C++11 Inheriting Constructor
    using Account::Account;

    ~Checking(){
    }

    void withdraw(float amount) override {
        if (balance_ - amount >= minimum_balance) {
            // Use scope resolution in order to call base class function from child class function.
            Account::withdraw(amount);
        } else {
            std::cout << "Balance would go under the threshold." << std::endl;
        }
    }
};

class Checking2 : public Account {
    float minimum_balance_;
public:
    Checking2(const std::string& name, float balance, float min_balance) : Account(name, balance), minimum_balance_(min_balance) {

    }

    ~Checking2(){
    }

    void withdraw(float amount) override {
        if (balance_ - amount >= minimum_balance_) {
            // Use scope resolution in order to call base class function from child class function.
            Account::withdraw(amount);
        } else {
            std::cout << "Balance would go under the threshold." << std::endl;
        }
    }

    float get_minimum_balance() const {
        return minimum_balance_;
    }
};

// This function/module is tightly coupled with Checking account.
// To perform the same transactions on other type of account, we'd need to write another function.
void perform_transactions(Checking* pAcc) {
    std::cout << "perform_transactions()" << std::endl;
    std::cout << "Initial balance = " << pAcc->get_balance() << std::endl;
    pAcc->deposit(100);
    pAcc->accumulate_interest();
    pAcc->withdraw(170);
    std::cout << "Interest rate = " << pAcc->get_interest_rate() << std::endl;
    std::cout << "Final balance = " << pAcc->get_balance() << std::endl;
}

// Pointer to or reference to base class can be assigned a pointer or reference to its child classes.
// This function/module knows only about base class, no child classes.
// This function demonstrates Polymorphism - when a message is sent to an object that represents different objects.
// Messages is passed to correct object. Code which uses polymorphism does not need to know the actual object on which
// functions will be called. This function would work without changing anything in it on any classes inherited from Account
// if we introduce them in future.
void perform_transactions2(Account* pAcc) {
    std::cout << "perform_transactions()" << std::endl;
    std::cout << "Initial balance = " << pAcc->get_balance() << std::endl;

    // To tell the compiler to call these functions not on the Account base object but on the actual children objects,
    // we need to mark them somehow: we need to use 'virtual' keyword.
    // This will actually allow child classes to override base class behaviour.
    pAcc->deposit(100);
    pAcc->accumulate_interest();
    pAcc->withdraw(170);

    std::cout << "Interest rate = " << pAcc->get_interest_rate() << std::endl;
    std::cout << "Final balance = " << pAcc->get_balance() << std::endl;
}

void demo_account() {
    Account acc("Bojan", 50);
    acc.deposit(100);
    acc.withdraw(30);

    Checking checking("Bojan", 100);
    checking.withdraw(51);
    perform_transactions(&checking);
    perform_transactions2(&checking);

    Savings savings("Bojan", 100, 0.05f);
    perform_transactions2(&savings);
    // Before marking Account class members as virtual, the output was:
    // perform_transactions()
    // Initial balance = 100
    // Interest rate = 0
    // Final balance = 30
}

void demo_virtual_destructors() {
    {
        Savings acc("Bojan", 100, 0.05f);
        perform_transactions2(&acc);

        // If we temporarily remove keyword virtual from all member functions of Account class the output is: 40
        // If we return keyword virtual to all member functions of Account class the output is: 48
        // This is because the size of Vptr is 8 bytes (on 64-bit platform).
        std::cout << "sizeof(Account) = " << sizeof(Account) << std::endl;

        // Both d-tors are called:
        // Savings::~Savings()
        // Account::~Account()
    }

    {
        // Before running this code remove keyword virtual from Account::~Account().

        Account* pAcc = new Savings("Bojan", 100, 0.05f);
        perform_transactions2(pAcc);
        delete pAcc;

        // problem:
        // Only Account::~Account() is called (because type of pAcc is pointer to Account).
        // Savings::~Savings() is not called!
        // We need to make Account::~Account() virtual!
        // Base class has to have a destructor declared as virtual!
        // Without a virtual destructor, the destructors of child will not execute if a pointer of base is deleted.
    }
}

// final (C++11)
// - Used to declare classes which cannot not be inherited.
class MyClass final {
public:
    MyClass(){}
    ~MyClass(){
        // release resources
    }
    void Foo1(const std::string& fileName) {}
    void Foo2(const std::string& fileName){}
    void Foo3(){}
};

//  error: cannot derive from ‘final’ base ‘class_demo::oop_demo::MyClass’ in derived type ‘class_demo::oop_demo::MyChildClass’
// class MyChildClass : public MyClass {};

class MyClass2 {
public:
    virtual void Foo1(float version){
        std::cout << "MyClass2::Foo1()" << std::endl;
    }

    virtual void Foo2(std::string arg){
        std::cout << "MyClass2::Foo2()" << std::endl;
    }

    void Foo3(std::string arg){
        std::cout << "MyClass2::Foo3()" << std::endl;
    }

    virtual void Foo4(std::string arg){
        std::cout << "MyClass2::Foo4()" << std::endl;
    }
};

class MyChildClass2 : public MyClass2 {
public:
    // Problem: We might make mistake with argument type and so this function is not overriding MyClass2::Foo1 as we intended
    // (as it does not have the same signature)! Compiler can't catch this subtle mistake. Therefore C++11 introduced keyword 'override'.
    void Foo1(int version){
        std::cout << "MyChildClass2::Foo1()" << std::endl;
    }

    // error: ‘void class_demo::oop_demo::MyChildClass2::Foo2(int)’ marked ‘override’, but does not override void Foo2(int arg) override {...}
    // void Foo2(int arg) override {
    //     std::cout << "MyChildClass2::Foo2()" << std::endl;
    // }

    void Foo2(std::string arg) override {
        std::cout << "MyChildClass2::Foo2()" << std::endl;
    }

    // It is not possible to override base function which is not declared as 'virtual'.
    // error: ‘void class_demo::oop_demo::MyChildClass2::Foo3(std::__cxx11::string)’ marked ‘override’, but does not override void Foo3(std::string arg) override
    // void Foo3(std::string arg) override {
    //     std::cout << "MyChildClass2::Foo3()" << std::endl;
    // }

    // If we want to prevent that further inheritors to override this function, we can declared it as 'final'
    void Foo4(std::string arg) override final {
        std::cout << "MyChildClass2::Foo4()" << std::endl;
    }
};

class MyGrandChildClass2 : public MyChildClass2 {
public:
    // We can't override function from parent class if that function was declared as 'final'.
    //  error: overriding final function ‘virtual void class_demo::oop_demo::MyChildClass2::Foo4(std::__cxx11::string)’ void Foo4(std::string arg) override final
    // void Foo4(std::string arg) override {
    //     std::cout << "MyGrandChildClass2::Foo4()" << std::endl;
    // }
};

void demo_overriding() {
    MyChildClass2 myChildObject;
    MyClass2& myClass = myChildObject;

    // calls MyClass2::Serialize()
    myClass.Foo1(3.14f);

    // calls MyChildClass2::Foo2()
    myClass.Foo2("test");
}

void demo_upcasting_downcasting() {
    Checking checkAcc("Bojan", 100);

    //
    // Upcasting
    //
    // - we upcast a child class object to base class object
    // - works automatically, no programmer's intervention is required
    // - it works only with pointers and references
    // - if pointers or references are not used => slicing
    Account* pAccount = &checkAcc;

    // It is not possible to assign pointer to base to pointer to child:
    // error: invalid conversion from ‘class_demo::oop_demo::Account*’ to ‘class_demo::oop_demo::Checking*’ [-fpermissive]
    // This makes sense as how can compiler know that pAccount points to some completely different child class e.g. Savings.
    // Checking* pChecking = pAccount;

    // But if we know that pAccount indeed points to Checking object, we can tell that to compiler:
    //
    // Downcasting
    //
    // - we downcasted base class pointer to a child class pointer
    // - programmer's intervention required: explicit casting
    Checking* pChecking = static_cast<Checking*>(pAccount);
}

void perform_transactions3(Account* pAcc) {
    std::cout << "perform_transactions()" << std::endl;
    std::cout << "Initial balance = " << pAcc->get_balance() << std::endl;

    pAcc->deposit(100);
    pAcc->accumulate_interest();
    // problem: we're blindly downcasrting pointer to base to pointer to one of the child classes.
    // If object is not of the assumed child class, we have undefined behaviour.
    Checking2* pChecking2 = static_cast<Checking2*>(pAcc);
    // we're calling a child class specific function on a downcasted pointer.
    // If original object is not of this child class type we have an undefined behaviour.
    std::cout << "Minimum balance of Checking2 account: " << pChecking2->get_minimum_balance() << std::endl;
    // Ideally, we'd somehow check if pAcc points to Checking2 account

    pAcc->withdraw(170);

    std::cout << "Interest rate = " << pAcc->get_interest_rate() << std::endl;
    std::cout << "Final balance = " << pAcc->get_balance() << std::endl;
}

void perform_transactions4(Account* pAcc) {
    std::cout << "perform_transactions()" << std::endl;
    std::cout << "Initial balance = " << pAcc->get_balance() << std::endl;

    pAcc->deposit(100);
    pAcc->accumulate_interest();

    // Using RTTI to determine the type of the underlying object:
    if (typeid(*pAcc) == typeid(Checking2)) {
        Checking2* pChecking2 = static_cast<Checking2*>(pAcc);
        std::cout << "Minimum balance of Checking2 account: " << pChecking2->get_minimum_balance() << std::endl;
    }

    // The same can be achieved with dynamic_cast<T>:
    Checking2* pChecking2 = dynamic_cast<Checking2*>(pAcc);
    if (pChecking2 != nullptr) {
        std::cout << "Minimum balance of Checking2 account: " << pChecking2->get_minimum_balance() << std::endl;
    }

    pAcc->withdraw(170);

    std::cout << "Interest rate = " << pAcc->get_interest_rate() << std::endl;
    std::cout << "Final balance = " << pAcc->get_balance() << std::endl;
}

//
// typeid
//
// - when used on non-polymorphic types, it gathers type information in compile-time
// - when used on polymorphic types, it will work in runtime
// - it makes sense to use it only for polymorphic types
//
// Ideally we should avoid using RTTI and dynamic_cast.
// For these to work compiler has to add extra information to polymorphic types.
// Compiler creates type_info object which contains type info and is stored along Vtable.
// typeid and dynamic_cast use this object to get information.
// dynamic_cast is slower than typeid as it has to go through class hierarchy in order to determine if conversion is possible.
// Avoid RTTI and rely on polymorphism.
void demo_RTTI(){
    Checking2 checkAcc("Bojan", 100, 50);
    perform_transactions3(&checkAcc);
    // Minimum balance of Checking2 account: 50

    Savings savAcc("Bojan - Savings", 145, 0.05f);
    perform_transactions3(&savAcc);
    // problem: this outputs:
    // Minimum balance of Checking2 account: 0.05
    // This is wrong output and we're lucky we didn't get crash here!

    int n{};
    float f{};
    const std::type_info& ti = typeid(n);
    std::cout << "Type name = " << ti.name() << std::endl;
    std::cout << "Type name = " << typeid(f).name() << std::endl;
    std::cout << "Type name = " << typeid(savAcc).name() << std::endl;

    Savings* pSavings = &savAcc;
    std::cout << "Type name = " << typeid(pSavings).name() << std::endl;
    std::cout << "Type name = " << typeid(*pSavings).name() << std::endl;

    Account* pAcccount = &savAcc;
    if (typeid(*pAcccount) == typeid(Savings)) {
        std::cout << "pAccount points to Savings object" << std::endl;
    } else {
        std::cout << "pAccount does not point to Savings object" << std::endl;
    }

// Type name = i
// Type name = f
// Type name = N10class_demo8oop_demo7SavingsE
// Type name = PN10class_demo8oop_demo7SavingsE

    perform_transactions4(&checkAcc);
    perform_transactions4(&savAcc);
}

class Document {
public:
    virtual void Serialize(float version) {
        std::cout << "Document::Serialize()" << std::endl;
    }
};

class Text : public Document {
public:
    void Serialize(float version) override final {
        std::cout << "Text::Serialize()" << std::endl;
    }
};

class RichText : public Text {
public:
    // cannot override as function is final in parent
    // void Serialize(float version) override final {
    //     std::cout << "RichText::Serialize()" << std::endl;
    // }
};

class XML : public Document {
public:
    // we don't override Document's Serialize() function
};

void Write(Document* pDoc) {
    pDoc->Serialize(1.1f);
}

// Problem solution: making base class abstract.
// Class is abstract if has at least one pure virtual function or if inherits from abstract class and it
// does not override at least one inherited pure virtual function.
// Abstract class:
// - can't be instantiated but pointers or references to it can.
// - can contain data, non-virtual functions...
// - establishes a contract with clients - like interface in C#, TypeScript or Go language
// - used for creatng interfaces.
//   Interface contains only pure virtual functions and no other members.
// Pure function:
// - virtual function marked with pure specifier (=0)
// - usually does not have an implementation (but it can have it)
// - cannot be invoked but if it has an implementation, it can be invoked only from derived classes.
// - they don't have an entry in Vtable but will define the layout of the functions in the Vtable of child classes.
// - must be overriden in derived classes. If not derived,derived class will also be an abstract one.
class Document2 {
public:
    // Pure virtual function.
    virtual void Serialize(float version) = 0;
};

class Text2 : public Document2 {
public:
    void Serialize(float version) override final {
        std::cout << "Text2::Serialize()" << std::endl;
    }
};

class RichText2 : public Text2 {
public:
    // cannot override as function is final in parent
    // void Serialize(float version) override final {
    //     std::cout << "RichText::Serialize()" << std::endl;
    // }
};

class XML2 : public Document2 {
public:
    // If we don't override Document2's Serialize() function
    // this class would also be abstract and trying to create an instance of it
    //      XML2 xml2;
    // would render the following compiler error:
    //      error: cannot declare variable ‘xml2’ to be of abstract type ‘class_demo::oop_demo::XML2’

    void Serialize(float version) override {
        std::cout << "XML2::Serialize()" << std::endl;
    }
};

void Write2(Document2* pDoc) {
    pDoc->Serialize(1.1f);
}

void demo_abstract_class(){
    Text t;
    Document& doc = t;
    doc.Serialize(1.2f);
    // output: Text::Serialize()

    XML xml;
    Write(&xml);
    // output: Document::Serialize()
    // Problem: child classes of Document are not forced to override Serialize()
    // Consequence: some child classes will not override it and clients which are
    // calling Serialize() on child classes will not be aware that default implementation
    // (from base class) is being used, which is wrong.
    // Solution: abstract class

    XML2 xml2;
    Write2(&xml2);
    // output: XML2::Serialize()
}

class Stream {
    std::string fileName_;
public:
    Stream(const std::string& fileName) : fileName_(fileName){
        std::cout << "Stream::Stream(const std::string& fileName)" << std::endl;
    }
    const std::string& GetFileName() const {
        return fileName_;
    }
    ~Stream() {
        std::cout << "Stream::~Stream()" << std::endl;
    }
};

class OutputStream : public Stream {
    std::ostream& out_;
public:
    OutputStream(std::ostream& out, const std::string& fileName): out_(out), Stream(fileName){
        std::cout << "OutputStream::OutputStream(...)" << std::endl;
    }
    std::ostream& operator<<(const std::string& data) {
        out_ << data;
        return out_;
    }
    ~OutputStream() {
        std::cout << "OutputStream::~OutputStream()" << std::endl;
    }
};

class InputStream : public Stream {
    std::istream& in_;
public:
    InputStream(std::istream& in, const std::string& fileName): in_(in), Stream(fileName){
        std::cout << "InputStream::InputStream(...)" << std::endl;
    }
    std::istream& operator>>(std::string& data) {
        in_ >> data;
        return in_;
    }
    ~InputStream() {
        std::cout << "InputStream::~InputStream()" << std::endl;
    }
};

class IOStream : public OutputStream, public InputStream {
public:
    IOStream(const std::string& fileName): OutputStream(std::cout, fileName), InputStream(std::cin, fileName) {
        std::cout << "IOStream::IOStream(...)" << std::endl;
    }
    ~IOStream() {
        std::cout << "IOStream::~IOStream()" << std::endl;
    }
};

// OutputStream2 virtually inherits Stream means that
// Vptr is added to OutputStream2 instance and it points to Stream.
class OutputStream2 : virtual public Stream {
    std::ostream& out_;
public:
    OutputStream2(std::ostream& out, const std::string& fileName): out_(out), Stream(fileName){
        std::cout << "OutputStream2::OutputStream2(...)" << std::endl;
    }
    std::ostream& operator<<(const std::string& data) {
        out_ << data;
        return out_;
    }
    ~OutputStream2() {
        std::cout << "OutputStream2::~OutputStream2()" << std::endl;
    }
};

// InputStream2 virtually inherits Stream means that
// Vptr is added to InputStream2 instance and it points to Stream.
class InputStream2 : virtual public Stream {
    std::istream& in_;
public:
    InputStream2(std::istream& in, const std::string& fileName): in_(in), Stream(fileName){
        std::cout << "InputStream2::InputStream2(...)" << std::endl;
    }
    std::istream& operator>>(std::string& data) {
        in_ >> data;
        return in_;
    }
    ~InputStream2() {
        std::cout << "InputStream2::~InputStream2()" << std::endl;
    }
};

// IOStream2 will contain an instance of OutputStream2 and InputStream2 and each of them will have
// Vptr pointint to the same Stream object.
// To get a single instance of Stream, compiler needs to call its c-tor from most derived class, IOStream2.
// OutputStream2 and InputStream2 c-tors will not be invoking Stream c-tor.
class IOStream2 : public OutputStream2, public InputStream2 {
public:
    // As IOStream2 inherits from two classes which virtually inherit Stream, this means IOStream2 will be directly
    // be responsible for constructing Stream class.
    // If we don't explicitily invoke Stream's parametrized c-tor, IOStream2 will implicitly invoke its default c-tor.
    // It is not available in our case and therefore we'd get an error:
    // error: no matching function for call to ‘class_demo::oop_demo::Stream::Stream()’
    // IOStream2(const std::string& fileName): OutputStream2(std::cout, fileName), InputStream2(std::cin, fileName) {
    //     std::cout << "IOStream2::IOStream2(...)" << std::endl;
    // }
    // Solution: explicitily invoke Stream's parametrized c-tor
    IOStream2(const std::string& fileName): OutputStream2(std::cout, fileName), InputStream2(std::cin, fileName), Stream(fileName) {
        std::cout << "IOStream2::IOStream2(...)" << std::endl;
    }
    ~IOStream2() {
        std::cout << "IOStream2::~IOStream2()" << std::endl;
    }
};

//
// Multiple Inheritance
//
// Class can inherit from multiple classes.
// Useful when class wants to inherit/use multiple behaviours.
// It can lead to Diamond Inheritance - when parent classes inherit the same parent
// thus forming a diamond shape in inheritance graph.
// E.g. InputStream and OutputStream inherit Stream and then IOStream inherits both InputStream and OutputStream.
// In the IOStream object there will be two instances of Stream.
//
void multiple_inheritance_demo() {
    IOStream iostream("test.txt");
    // Output (note the problem: Stream c-tor is invoked twice):
    //
    // Stream::Stream(const std::string& fileName)
    // OutputStream::OutputStream(...)
    // Stream::Stream(const std::string& fileName)
    // InputStream::InputStream(...)
    // IOStream::IOStream(...)
    // IOStream::~IOStream()
    // InputStream::~InputStream()
    // Stream::~Stream()
    // OutputStream::~OutputStream()
    // Stream::~Stream()
}

//
void multiple_inheritance_demo2() {
    IOStream iostream("test.txt");
    std::string data;
    iostream >> data;
    iostream << data;

    // Problem: Compiler does not know on which of 2 inherited instances of Stream to call the function:
    // error: request for member ‘GetFileName’ is ambiguous
    // iostream << iostream.GetFileName() << std::endl;
    // Solution: make sure there is only one instance of Stream in IOStream (by using Virtual Inheritance)
}

void multiple_inheritance_solution_demo(){
    IOStream2 iostream("test.txt");
    iostream << iostream.GetFileName() << std::endl;
    //
    //  Output: note that Stream is constructed only once and that GetFileName() call is not ambiguous anymore.
    //
    // Stream::Stream(const std::string& fileName)
    // OutputStream2::OutputStream2(...)
    // InputStream2::InputStream2(...)
    // IOStream2::IOStream2(...)
    // test.txt
    // IOStream2::~IOStream2()
    // InputStream2::~InputStream2()
    // OutputStream2::~OutputStream2()
    // Stream::~Stream()
}

} // namespace oop_demo


void run() {
    std::cout << "\n\n ***** class_demo::run() ***** \n\n" << std::endl;
    // class_demo();
    // struct_demo();
    // copy_constructor_demo();
    // delegating_constructors_demo();
    // default_and_deleted_member_functions_demo();
    // friend_demo();
    // initialization_vs_assignment::demo();
    // oop_demo::demo();
    // oop_demo::demo_account();
    // oop_demo::demo_virtual_destructors();
    // oop_demo::demo_overriding();
    // oop_demo::demo_RTTI();
    // oop_demo::demo_abstract_class();
    // oop_demo::multiple_inheritance_demo();
    // oop_demo::multiple_inheritance_demo2();
    oop_demo::multiple_inheritance_solution_demo();
}

}
