cmake_minimum_required(VERSION 3.17)
project(Cormoran C)

set(CMAKE_C_STANDARD 99)

add_executable(Cormoran main.c resolution/resolution.h resolution/resolution.c)