#include <iostream>
#include <initialization_demo.hpp>
#include <declarations_demo.hpp>

int main(int argc, char const *argv[]) {
  std::cout << "Hello Docker container!" << std::endl;
  if (int n = 0; n < 1) {
      std::cout << "Your compiler supports C++17." << std::endl;
  }
  declarations_demo();
  initialization_demo();
  return 0;
}
