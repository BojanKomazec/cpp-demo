cmake_minimum_required(VERSION 3.10.2)
project (cpp-demo)

#Bring the headers, such as Student.h into the project
include_directories(include)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(cpp-demo main.cpp ${SOURCES})
target_link_libraries(${PROJECT_NAME} stdc++fs)

# Add -O0 to remove optimizations when using gcc
IF(CMAKE_COMPILER_IS_GNUCC)
    # set C++ standard
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++14")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17")

    # disable compiler optimizations
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")


    # Disable copy elision
    # Valid only if std < c++17 as from C++17, copy elision is guaranteed.
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-elide-constructors")

    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
#   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

# set the compilation mode to Debug (non-optimized code with debug symbols)
set(CMAKE_BUILD_TYPE Debug)

# set(MY_CUSTOM_PREPROCESSOR_SYMBOL "SomeRandomValueOfMyCutomPreprocessorSymbol")
add_definitions(-DPREPROCESSOR_DIAGNOSTICS -DMY_CUSTOM_PREPROCESSOR_SYMBOL="SomeRandomValueOfMyCutomPreprocessorSymbol")
