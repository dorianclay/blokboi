cmake_minimum_required(VERSION 3.10)

# Project name
project(Blokboi)


add_subdirectory(src)
# # Executable
# add_executable(blokboi ./src/game.cpp ${HEADER_FILES})

# # Flags
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)
# if(MSVC)
#   target_compile_options(blokboi PRIVATE /W4 /WX)
# else()
#   target_compile_options(blokboi PRIVATE -Wall -Wextra -Wpedantic)
# endif()

# # Files
# set(DIRECTORIES src)
# set(HEADER_FILES ${DIRECTORIES}/player_controller.h
#                  ${DIRECTORIES}/scene.h
#                  ${DIRECTORIES}/player.h
#                  ${DIRECTORIES}/block.h 
#                  ${DIRECTORIES}/game_object.h
#                  )
# target_include_directories(blokboi PRIVATE ${DIRECTORIES})

# Includes

###########################################

# External Libraries - 3rd party libs
# find_package(Threads)
# add_subdirectory(./lib/loguru)
# target_link_libraries(blokboi PUBLIC Loguru ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})
# target_include_directories(blokboi PUBLIC
#                           "${PROJECT_BIN_DIR}"
#                           "./lib/loguru"
#                           )
