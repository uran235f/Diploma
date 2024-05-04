cmake_minimum_required(VERSION 3.22)
project(optimizer)
list(APPEND CMAKE_PREFIX_PATH "/usr/share/doc/libpqxx")

set(CXX_STANDARD_REQUIRED 17)
set(CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 17)

message("Compiler: ${CMAKE_C_COMPILER}")
message("Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
find_library(PQXX_LIB pqxx)

add_executable(${PROJECT_NAME} main.cpp)

set(CMAKE_INCLUDE_CURRENT_DIR, include)
set(CMAKE_CURRENT_SOURCE_DIR, src)

add_subdirectory(include)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} ${PQXX_LIB})