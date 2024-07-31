cmake_minimum_required(VERSION 3.20)
project(simulator)

set(CMAKE_CXX_STANDARD 20)
file(GLOB_RECURSE sources "src/*.cpp")
# file(GLOB_RECURSE sources "demo/modules.cpp")

include_directories(include)

add_executable(code ${sources})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# cmake_minimum_required(VERSION 3.20)
# project(simulator)

# set(CMAKE_CXX_STANDARD 20)
# file(GLOB_RECURSE sources "src/*.cpp")
# # file(GLOB_RECURSE sources "demo/modules.cpp")

# include_directories(include)

# add_executable(code ${sources})

# # 移除之前的优化标志
# unset(CMAKE_CXX_FLAGS CACHE)

# # 设置构建类型为Debug
# set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)