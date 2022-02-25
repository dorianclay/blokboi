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
add_executable(blokboi ./src/game.cpp)


# External Libraries - 3rd party libs
# find_package(Threads)
# add_subdirectory(./lib/loguru)
# target_link_libraries(blokboi PUBLIC Loguru ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})
# target_include_directories(blokboi PUBLIC
#                           "${PROJECT_BIN_DIR}"
#                           "./lib/loguru"
#                           )
