cmake_minimum_required(VERSION 3.9)
project(SwiftRT)

set(CMAKE_CXX_FLAGS "/std:c++17 /O2 ${CMAKE_CXX_FLAGS}")

include_directories(SwiftRT "include")
include_directories(SwiftRT "src")

file(GLOB SRCS "src/*.cpp")
file(GLOB LIBS "include/*.cpp")

add_executable(SwiftRT ${SRCS} ${LIBS} ${RT} ${IMGUI})

find_package(OpenMP)

if(OpenMP_CXX_FOUND)
    target_link_libraries(SwiftRT OpenMP::OpenMP_CXX)
endif()

target_link_libraries(SwiftRT "../lib/OpenImageDenoise")
target_link_libraries(SwiftRT "../lib/tbb")
target_link_libraries(SwiftRT "../lib/tbbmalloc")