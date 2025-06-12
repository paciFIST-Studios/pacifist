cmake_minimum_required(VERSION 3.31)
project(paciFIST)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_LINK_WARNING_AS_ERROR ON)


add_subdirectory(assets)
add_subdirectory(vendor)
add_subdirectory(src)

