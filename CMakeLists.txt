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
)


# Flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-fPIC")

option(VERBOSE_WARN "Show detailed warning information" ON)







option(PYTHON_EXT "Use Pybind11 to generate Python extension module" OFF)
if(PYTHON_EXT)
  find_package(pybind11 CONFIG)
  pybind11_add_module(blokboi lib/cgame.cpp ${SOURCE_FILES})
else()
  add_executable(blokboi lib/game_manager.cpp ${SOURCE_FILES})

endif()





if(VERBOSE_WARN)
  if(MSVC)
    target_compile_options(blokboi PRIVATE /W4 /WX)
  else()
    target_compile_options(blokboi PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endif()


# External Libraries - 3rd party libs
find_package(Threads)

add_library(Loguru include/loguru/loguru.cpp)
target_include_directories(Loguru INTERFACE include/loguru)
install(TARGETS Loguru DESTINATION lib)
install(FILES include/loguru/loguru.hpp DESTINATION include)


add_subdirectory(include/random)
target_link_libraries(blokboi PUBLIC 
                      Loguru
                      effolkronium_random
                      ${CMAKE_THREAD_LIBS_INIT}
                      ${CMAKE_DL_LIBS})

install(TARGETS blokboi DESTINATION bin)