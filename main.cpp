#include <iostream>
#include <iostream_demo.hpp>
#include <initialization_demo.hpp>
#include <declarations_demo.hpp>
#include <recursion_demo.hpp>
#include <static_demo.hpp>
#include <strings_demo.hpp>

int main(int argc, char const *argv[]) {
  std::cout << "Hello Docker container!" << std::endl;
  if (int n = 0; n < 1) {
      std::cout << "Your compiler supports C++17." << std::endl;
  }
  declarations_demo();
  initialization_demo();
  iostream_demo::run();
  recursion_demo::run();
  static_demo::run();
  strings_demo::run();
  return 0;
}
