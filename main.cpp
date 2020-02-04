#include <iostream>
#include <iostream_demo.hpp>
#include <initialization_demo.hpp>
#include <declarations_demo.hpp>
#include <lambda_demo.hpp>
#include <pointer_demo.hpp>
#include <recursion_demo.hpp>
#include <reference_demo.hpp>
#include <static_demo.hpp>
#include <strings_demo.hpp>

int main(int argc, char const *argv[]) {
  std::cout << "main()" << std::endl;
  if (int n = 0; n < 1) {
      std::cout << "Your compiler supports C++17." << std::endl;
  }

  declarations_demo::run();
  initialization_demo::run();
  iostream_demo::run();
  lambda_demo::run();
  pointer_demo::run();
  recursion_demo::run();
  reference_demo::run();
  static_demo::run();
  strings_demo::run();

  return 0;
}
