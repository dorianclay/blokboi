cmake_minimum_required(VERSION 3.10)

# Project name
project(Blokboi)

# Source Files
set(SOURCE_FILES
    src/scene.cpp
    src/block.cpp
    src/player.cpp
)

# Executable
add_executable(blokboi src/game.cpp ${SOURCE_FILES})

# Flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(VERBOSE_WARN "Show detailed warning information" ON)
if(VERBOSE_WARN)
  if(MSVC)
    target_compile_options(blokboi PRIVATE /W4 /WX)
  else()
    target_compile_options(blokboi PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endif()

# Include directories
set(DIRECTORIES src)
target_include_directories(blokboi PRIVATE ${DIRECTORIES})

# Includes

###########################################

# External Libraries - 3rd party libs
find_package(Threads)

add_library(Loguru include/loguru/loguru.cpp)
target_include_directories(Loguru INTERFACE include/loguru)
install(TARGETS Loguru DESTINATION include)
install(FILES include/loguru.hpp DESTINATION include)
add_subdirectory(include/random)
target_link_libraries(blokboi PUBLIC 
                      Loguru
                      effolkronium_random
                      ${CMAKE_THREAD_LIBS_INIT}
                      ${CMAKE_DL_LIBS})

install(TARGETS blokboi DESTINATION bin)
