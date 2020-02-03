# cpp-demo

C++11/14/17 terminal application which demonstrates various features of modern C++.

# Compiling the project

## Compiling on the native OS

Verify that you have installed cmake and make:

```
$ cmake --version
$ make --version
```

Example:

```
$ cmake --version
cmake version 3.10.2

CMake suite maintained and supported by Kitware (kitware.com/cmake).

$ make --version
GNU Make 4.1
Built for x86_64-pc-linux-gnu
Copyright (C) 1988-2014 Free Software Foundation, Inc.
Licence GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
```

To check which C++ compiler will be used, open CMakeCache.txt and find lines like:
```
//CXX compiler
CMAKE_CXX_COMPILER:FILEPATH=/usr/bin/c++
```

From `man gcc`:

```
gcc - GNU project C and C++ compiler.
When you compile C++ programs, you should invoke GCC as g++ instead.
```


$ gcc --version
gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

$ which gcc
/usr/bin/gcc


$ /usr/bin/gcc --version
gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

$ /usr/bin/c++ --version
c++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

$ which c++
/usr/bin/c++

$ which g++
/usr/bin/g++

$ which gcc
/usr/bin/gcc

$ readlink /usr/bin/c++
/etc/alternatives/c++

$ readlink /etc/alternatives/c++
/usr/bin/g++

$ readlink /usr/bin/g++
g++-7
```

Run:
```
$ cmake . && make
```

## Compiling in Docker image

Compiling the project takes place during the build of the application's Docker image:
```
$ docker build -t cpp-demo .
```

# Running the executable

## Running the build created on the native OS

Run:
```
$ ./cpp-demo
```

## Runnig the build in Docker container

Project's executable is set as the application to be run upon launching its Docker container:
```
$ docker run --rm --name cpp-demo cpp-demo
```
Building and running Docker image in a single command:
```
$ docker build -t cpp-demo . && docker run --rm --name cpp-demo cpp-demo
```

# References
[C++ development with Docker containers in Visual Studio Code | C++ Team Blog](https://devblogs.microsoft.com/cppblog/c-development-with-docker-containers-in-visual-studio-code/)
