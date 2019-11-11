#
# Preparing Docker image with CLang
#

# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest as clang_ready

# Update apps on the base image
RUN apt-get -y update && apt-get install -y

# Install the Clang compiler
RUN apt-get -y install clang

#
# Compiling the project
#

FROM clang_ready

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY . /usr/src/cpp-demo

# Specify the working directory
WORKDIR /usr/src/cpp-demo

RUN clang --version

# Use Clang to compile the Test.cpp source file
RUN clang++ -std=c++17 -o cpp-demo main.cpp

# Run the output program from the previous step
CMD ["./cpp-demo"]
