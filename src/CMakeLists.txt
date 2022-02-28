
set(HEADER_FILES
    player_controller.h
    scene.h
    player.h
    block.h 
    game_object.h
)

add_executable(blokboi game.cpp ${HEADER_FILES})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if(MSVC)
  target_compile_options(blokboi PRIVATE /W4 /WX)
else()
  target_compile_options(blokboi PRIVATE -Wall -Wextra -Wpedantic)
endif()
