cmake_minimum_required(VERSION 3.14)
project(hw02)
set(CMAKE_CXX_STANDARD 20)

add_library(hw02 combinatorics.cpp)
target_include_directories(hw02 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(runhw02 run.cpp)
target_link_libraries(runhw02 hw02)