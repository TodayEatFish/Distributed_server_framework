cmake_minimum_required(VERSION 3.1)
project(Distributed_server_framework)

set(CMAKE_CXX_STANDARD 17)

add_executable(Distributed_server_framework main.cpp log.hpp log.cpp singleton.h test.cpp)
add_executable(UTIL util.cpp util.h)
