cmake_minimum_required(VERSION 3.10)

# Project name
project(blokboi)

# Flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Files

# Includes

include_directories( ./include ./src )

# Executable
add_executable(Tutorial tutorial.cpp)


# External Libraries - 3rd party libs
