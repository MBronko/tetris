cmake_minimum_required(VERSION 3.17)
project(tetris C)

set(CMAKE_C_STANDARD 11)

add_executable(tetris src/main.c src/components/game.c src/components/menu.c src/draw/draw-menu.c src/draw/draw-game.c src/tools/draw-tools.c src/tools/game-tools.c src/tools/menu-tools.c)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
target_link_libraries(tetris m)
target_link_libraries(tetris ncursesw)
target_link_libraries(tetris pthread)