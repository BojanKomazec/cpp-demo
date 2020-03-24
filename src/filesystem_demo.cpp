#include <filesystem_demo.hpp>
#include <iostream>
#include <cassert>
#include <filesystem>

namespace filesystem_demo {

using namespace std::filesystem;

//
// path class (C++17)
//
// - unix or windows style paths
// - directory or file path
// - does not check whether path is valied; value can be any string
//
void path_demo() {
    path pathUnix{R"(/home/Bojan/Downloads)"};
    path pathUnix2{R"(/home/Bojan/Downloads/books.zip)"};
    // path has to terminate with path segment separator in order to be considered a directory path
    path pathUnix3{R"(/home/Bojan/Downloads/)"};
    path pathWin{R"(c:\\Users\\Bojan\\Downloads)"};


    std::cout << pathUnix.string() << " has file name: " << pathUnix.has_filename() << std::endl; // 1
    std::cout << pathUnix2.string() << " has file name: " << pathUnix2.has_filename() << std::endl; // 1
    std::cout << pathUnix3.string() << " has file name: " << pathUnix3.has_filename() << std::endl; // 0

    for (const auto& segment : pathUnix2) {
        std::cout << segment << std::endl;
    }
    // Output:
    // "/"
    // "home"
    // "Bojan"
    // "Downloads"
    // "books.zip"

    std::cout << "Current path: " << std::filesystem::current_path() << std::endl;
}

//
// directory_iterator (C++17)
//
// - allows iterating over the content of the directory
//
void directory_iterator_demo(){
    path pathUnix{R"(/home/bojan/Downloads/)"};
    directory_iterator beg{pathUnix};
    directory_iterator end{};

    while (beg != end) {
        std::cout << *beg << std::endl;
        ++beg;
    }
}

void run() {
    std::cout << "filesystem_demo::run()" << std::endl;
    path_demo();
    directory_iterator_demo();
}

}
