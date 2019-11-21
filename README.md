# cpp-demo
Dockerized C++11/17 demo

## Compiling the project
Compiling the project takes place during the build of the application's Docker image:
```
$ docker build -t cpp-demo .
```

## Running the executable
Project's executable is set as the application to be run upon launching its Docker container:
```
$ docker run --rm --name cpp-demo cpp-demo
```
Building and running Docker image is unified in Makefile target so the easiest way to run this app is to simply execute:
```
$ make docker-run
```

## References
[C++ development with Docker containers in Visual Studio Code | C++ Team Blog](https://devblogs.microsoft.com/cppblog/c-development-with-docker-containers-in-visual-studio-code/)
