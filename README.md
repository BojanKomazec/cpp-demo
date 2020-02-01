# cpp-demo

C++11/14/17 terminal application which demonstrates various features of modern C++.

# Compiling the project

## Compiling on the native OS

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
