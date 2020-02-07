#include <class_demo.hpp>
#include <iostream>
#include <cassert>

namespace class_demo {

class Car {
    float fuel;
    float speed;
    int passengers;
public:
    // Constructor.
    // Can have arguments and be overloaded.
    // Default c-tor is the one with no arguments.
    // Compiler creates default c-tor if there are no other c-tors defined.
    Car() {
        fuel = 0;
        speed = 0;
        passengers = 0;
        std::cout << "Car::Car()" << std::endl;
    }

    Car(float amount) {
        std::cout << "Car::Car(float)" << std::endl;
        fuel = amount;
    }

    // Destructor is used to free resources when object is destroyed.
    // Has no arguments, can't be overloaded.
    // automatically called when object is deleted from heap or goes out from scope on stack.
    ~Car() {
        std::cout << "Car::~Car()" << std::endl;
    }
    void FillFuel(float amount);
    void Accelerate();
    void Brake();
    void AddPassengers(int count);
    void Dashboard();
};

void Car::FillFuel(float amount) {
    fuel = amount;
}

void Car::Accelerate(){
    speed++;
    fuel -= 0.5f;
}

void Car::AddPassengers(int count) {
    passengers += count;
}

void Car::Brake(){
    speed = 0;
}

// If class members are not initialized, they will by default have random values (not default values for their types).
// E.g.
// Fuel = 3.08075e-41
// Speed = 1.97402e-08
// Passengers = 32764
void Car::Dashboard() {
    std::cout << "Fuel = " << fuel << "\n";
    std::cout << "Speed = " << speed << "\n";
    std::cout << "Passengers = " << passengers << "\n";
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
    car.FillFuel(6);
    car.Dashboard();
    car.Accelerate();
    car.Accelerate();
    car.Accelerate();
    car.Accelerate();
    car.Dashboard();
    car.Brake();
    car.Dashboard();
}

// all struct members are always public
struct Point {
    int x;
    int y;
}

void struct_demo() {
    Point p;
    p.x = 12;
    p.y = 13;
    std::cout << "Point coordinates: (" << p.x << ", " << p.y << ")" << std::endl;
}

void run() {
    std::cout << "class_demo::run()" << std::endl;
    class_demo();
    struct_demo();
}

}
