cmake_minimum_required(VERSION 3.26)
project(py3dmath C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(src/headers)

add_library(py3dmath STATIC
    src/source/quaternion.c
    src/source/util.c
    src/source/vector.c
    src/source/matrix.c
)
