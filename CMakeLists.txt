cmake_minimum_required(VERSION 3.10)

# Project name
project(Blokboi LANGUAGES CXX)

# Source Files
set(SOURCE_FILES
    lib/game.cpp
    lib/scene.cpp
    lib/block.cpp
    lib/player.cpp
    lib/player_controller.cpp
    lib/generators.cpp
)


# Flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-fPIC")

option(VERBOSE_WARN "Show detailed warning information" ON)

# Create the module or executable we'll be using
option(PYTHON_EXT "Use Pybind11 to generate Python extension module" ON)
option(PY_GAME_OBJ "Geneate a Python extension module for game objects" OFF)
if(PYTHON_EXT)
  find_package(pybind11 CONFIG)
  pybind11_add_module(blokboi lib/cgame.cpp ${SOURCE_FILES})
  if(PY_GAME_OBJ)
    pybind11_add_module(game_object lib/cgame_object.cpp ${SOURCE_FILES})
  endif()
else()
  add_executable(blokboi lib/game_manager.cpp ${SOURCE_FILES})
  install(TARGETS blokboi DESTINATION bin)
endif()


# Set compliler warnings, if applicable
if(VERBOSE_WARN)
  if(MSVC)
    target_compile_options(blokboi PRIVATE /W4) # /WX
  else()
    target_compile_options(blokboi PRIVATE -Wall) # -Wextra -Wpedantic
  endif()
endif()


# External Libraries
find_package(Threads)
add_library(Loguru include/loguru/loguru.cpp)
target_include_directories(Loguru INTERFACE include/loguru)
install(TARGETS Loguru DESTINATION lib)
install(FILES include/loguru/loguru.hpp DESTINATION include)

# Link libraries
add_subdirectory(include/random)
target_link_libraries(blokboi PUBLIC
                      Loguru
                      effolkronium_random
                      ${CMAKE_THREAD_LIBS_INIT}
                      ${CMAKE_DL_LIBS})
if(PYTHON_EXT)
  if(PY_GAME_OBJ)
    target_link_libraries(game_object PUBLIC
                          Loguru
                          effolkronium_random
                          ${CMAKE_THREAD_LIBS_INIT}
                          ${CMAKE_DL_LIBS})
  endif()
endif()
