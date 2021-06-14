cmake_minimum_required(VERSION 3.10)

# set the project name
project(BatchExec)

# Find dependencies
find_package(MPI REQUIRED COMPONENTS CXX)

# Add src
add_subdirectory(src)

# Add tests
enable_testing()
add_subdirectory(tests)