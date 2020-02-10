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

void run() {
    std::cout << "class_demo::run()" << std::endl;
    class_demo();
    struct_demo();
    copy_constructor_demo();
    delegating_constructors_demo();
}

}
