cmake_minimum_required(VERSION 3.10)
project(Raytracer)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include/ "externals/glm" "externals/utilities")

file(GLOB SOURCE include/*.h src/*.cpp)

add_executable(Raytracer ${SOURCE})