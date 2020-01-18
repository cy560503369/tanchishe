cmake_minimum_required(VERSION 3.5)
project(tanchishe)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(tanchishe main.cpp snack.cpp snack.h)
