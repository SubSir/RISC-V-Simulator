cmake_minimum_required(VERSION 3.20)
project(simulator)

set(CMAKE_CXX_STANDARD 20)
file(GLOB_RECURSE sources "src/*.cpp")
# file(GLOB_RECURSE sources "demo/modules.cpp")

include_directories(include)

add_executable(modules ${sources})

target_compile_definitions(modules PRIVATE _DEBUG)