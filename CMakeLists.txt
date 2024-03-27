cmake_minimum_required(VERSION 3.0)
project(DumbSThreeMHeaderEditor)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(DumbSThreeMHeaderEditor main.c)
# https://stackoverflow.com/a/50882216
if(MSVC)
  target_compile_options(DumbSThreeMHeaderEditor PRIVATE /W4 /WX)
else()
  target_compile_options(DumbSThreeMHeaderEditor PRIVATE -Wall -Wextra -Wpedantic -Werror -march=native)
endif()