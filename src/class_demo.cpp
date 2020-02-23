#include <class_demo.hpp>
#include <iostream>
#include <cassert>

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

void run() {
    std::cout << "\n\n ***** class_demo::run() ***** \n\n" << std::endl;
    class_demo();
    struct_demo();
    copy_constructor_demo();
    delegating_constructors_demo();
    default_and_deleted_member_functions_demo();
    friend_demo();
    initialization_vs_assignment::demo();
}

}
