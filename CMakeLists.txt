cmake_minimum_required(VERSION 3.17)
project(Cormoran C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)
add_executable(Cormoran main.c exec/exec.h exec/exec.c utils/utils.h utils/utils.c builtin/builtin.h builtin/builtin.c)
target_link_libraries(Cormoran ncurses)
