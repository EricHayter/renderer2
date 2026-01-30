cmake_minimum_required(VERSION 3.25)

project(renderer2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(thirdparty)
add_subdirectory(src)
