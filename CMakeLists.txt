cmake_minimum_required(VERSION 3.0)
project(dstmhe)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(dstmhe main.c)
# https://stackoverflow.com/a/50882216
if(MSVC)
  target_compile_options(dstmhe PRIVATE /W4 /WX)
elseif(GNU)
  target_compile_options(dstmhe PRIVATE -Wall -Wextra -Wpedantic -Werror -march=native)
endif()