cmake_minimum_required(VERSION 3.22)
project(optimizer)

set(CXX_STANDARD_REQUIRED 17)
set(CXX_STANDARD_REQUIRED TRUE)

find_package(Boost REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

set(CMAKE_INCLUDE_CURRENT_DIR, include)
set(CMAKE_CURRENT_SOURCE_DIR, src)

add_subdirectory(include)
add_subdirectory(src)