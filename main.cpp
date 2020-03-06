#include <iostream>
#include <iostream_demo.hpp>
#include <class_demo.hpp>
#include <declarations_demo.hpp>
#include <dynamic_memory_management_demo.hpp>
#include <enum_demo.hpp>
#include <functions_demo.hpp>
#include <initialization_demo.hpp>
#include <lambda_demo.hpp>
#include <operators_demo.hpp>
#include <pointer_demo.hpp>
#include <preprocessor_demo.hpp>
#include <recursion_demo.hpp>
#include <reference_demo.hpp>
#include <smart_pointers_demo.hpp>
#include <statements_demo.hpp>
#include <static_demo.hpp>
#include <strings_demo.hpp>
#include <string_streams_demo.hpp>
#include <type_conversions_demo.hpp>
#include <utility_demo.hpp>

int main(int argc, char const *argv[]) {
  std::cout << "main()" << std::endl;
  if (int n = 0; n < 1) {
      std::cout << "Your compiler supports C++17." << std::endl;
  }

  if (true) {
    declarations_demo::run();
  } else {
    class_demo::run();
    declarations_demo::run();
    dynamic_memory_management_demo::run();
    enum_demo::run();
    functions_demo::run();
    initialization_demo::run();
    iostream_demo::run();
    lambda_demo::run();
    operators_demo::run();
    pointer_demo::run();
    preprocessor_demo::run();
    recursion_demo::run();
    reference_demo::run();
    smart_pointers_demo::run();
    static_demo::run();
    statements_demo::run();
    strings_demo::run();
    string_streams_demo::run();
    type_conversions_demo::run();
    utility_demo::run();
  }

  return 0;
}
