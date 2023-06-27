cmake_minimum_required(VERSION 3.0)
project(try_catch_block)

set(CMAKE_CXX_STANDARD 11)

include_directories(exception_handler.h)
add_executable(try_catch_block samples.cpp)

target_link_libraries(try_catch_block pthread)