cmake_minimum_required(VERSION 3.27)
project(py3dmath C)

set(CMAKE_C_STANDARD 23)

find_package(Python COMPONENTS Development)

add_library(py3dmath STATIC mathmodule.c src/source/py3dvector3.c src/source/py3dquaternion.c src/source/py3dmatrix4x4.c)
include_directories(src/headers)
include_directories(../lib/src/headers)
link_directories(../lib/cmake-build-debug)

target_link_libraries(py3dmath Python::Python libpy3dmath)