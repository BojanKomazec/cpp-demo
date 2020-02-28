#include <enum_demo.hpp>
#include <iostream>
#include <cassert>

namespace enum_demo {

// these are valid colours
#define COLOUR_RED 0
const int COLOUR_GREEN = 1;

void paint(int colour) {
    std::cout << "paint(): colour = " << colour << std::endl;

    if (colour == COLOUR_RED) {
        std::cout << "painting in RED" << std::endl;
    } else if (colour == COLOUR_GREEN){
        std::cout << "painting in GREEN" << std::endl;
    } else {
        std::cout << "Unknown colour!" << std::endl;
    }
}

void show_problem() {
    paint(COLOUR_RED);
    paint(COLOUR_GREEN);
    // Problem: we can pass any random integer.
    // Ideally, we want to prevent this and allow passing only values from predefined set.
    paint(3);
}

//
// Enumerated Type
//
// - created via enum keyword
// - predefined finite set of values called symbolic constants or enumerators
// - enumerators are internally represented as integers
// - enumerators can implicitly be converted to integers but integers can't be implicitly converted to enums
// - default value of the first enumerator is 0 but any number can be assigned
// - subsequent enumerators have values increased by 1

enum Colour {
    RED,   // 0
    GREEN, // 1
    BLUE   // 2
};

void paint2(Colour colour) {
    std::cout << "paint2(): colour = " << colour << std::endl;

    if (colour == RED) {
        std::cout << "painting in RED" << std::endl;
    } else if (colour == GREEN){
        std::cout << "painting in GREEN" << std::endl;
    } else if (colour == BLUE) {
        std::cout << "painting in BLUE" << std::endl;
    } else {
        std::cout << "Unknown colour!" << std::endl;
    }
}

void show_solution(){
    Colour c = RED;

    // error: invalid conversion from ‘int’ to ‘enum_demo::Colour’ [-fpermissive]
    // c = 1;

    int n = GREEN;
    assert(n == 1);

    paint2(c);

    paint2(BLUE);

    // error: invalid conversion from ‘int’ to ‘enum_demo::Colour’ [-fpermissive]
    // paint2(1);

    paint2(static_cast<Colour>(1));
}

// The scope of enumerators (e.g. RED) is the same as scope of their enum type.
// They can't be re-declared.
// Problem: we might want to use the same, native names for enumerators.
enum TrafficLight {
    // error: redeclaration of ‘RED’
    // RED,
    TL_RED,
    // error: redeclaration of ‘GREEN’
    // GREEN,
    TL_GREEN,
    TL_YELLOW
};

// Solution:
//
// Scoped Enumerator Types
//
// Enumerators are declared only within the scope.
// Enumerators's scope has to be resolved.
// Coped enumerators can't be implicitly converted to int.
// int is default underlying type.
// Scoped Enumerator Types are preferred over regular ones.
enum class TrafficLight2 {
    RED,
    GREEN,
    YELLOW
};

void scoped_enums_demo() {
    TrafficLight2 tl = TrafficLight2::RED;

    // error: cannot convert ‘enum_demo::Colour’ to ‘enum_demo::TrafficLight2’ in assignment
    // tl = GREEN;

    // error: cannot convert ‘enum_demo::TrafficLight2’ to ‘int’ in initialization
    // int n = TrafficLight2::GREEN;
    int n = static_cast<int>(TrafficLight2::GREEN);
    assert(n == 1);
}

// It is possible to specify some other integral type as underlying type:
enum class TrafficLight3 : char {
    RED = 'r',
    GREEN,
    YELLOW
};



void run() {
    std::cout << "enum_demo::run()" << std::endl;
    show_problem();
    show_solution();
    scoped_enums_demo();
}

}
