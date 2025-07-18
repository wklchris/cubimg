cmake_minimum_required(VERSION 3.20)
project(cubimg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)

file(GLOB SOURCE_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_CPP_FILES})

target_link_libraries(${PROJECT_NAME} raylib)