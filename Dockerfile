#
# Preparing Docker image with CLang
#

# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest as clang_ready

# Update apps on the base image
RUN apt-get -y update && apt-get install -y

# Install the Clang compiler
RUN apt -y install clang
RUN apt -y install cmake

#
# Compiling the project
#

FROM clang_ready

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY . /usr/src/cpp-demo

# Specify the working directory
WORKDIR /usr/src/cpp-demo

# Use Clang to compile the project
# RUN clang --version
# RUN clang++ -std=c++17 -Iinclude -o cpp-demo ./src/initialization_demo.cpp ./src/declarations_demo.cpp main.cpp

# Use cmake to compile the project with CXX compiler it finds on the system (and that should be clang)
RUN cmake --version
RUN ls -la
RUN cmake .
RUN make

# Run the output program from the previous step
CMD ["./cpp-demo"]
