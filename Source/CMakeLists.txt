cmake_minimum_required(VERSION 2.8.4)
project(RealRT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(PNG REQUIRED)

add_subdirectory(RTEngine)
add_subdirectory(SphereTest)
