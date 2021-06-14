cmake_minimum_required(VERSION 3.10)

# set the project name
project(BatchExec)

# Global compiler flags
if (CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

# Find dependencies
find_package(MPI REQUIRED COMPONENTS CXX)

# Add src
add_subdirectory(src)

# Add tests
enable_testing()
add_subdirectory(tests)