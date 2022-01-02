cmake_minimum_required(VERSION 3.10)
project(Raytracer)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)


find_path(STB_INCLUDE_DIRS "stb.h")
find_package(fmt CONFIG REQUIRED)
file(GLOB_RECURSE src_files include/*.h src/*.cpp)
add_library(RayLib STATIC ${src_files})
target_include_directories(RayLib PUBLIC "include/")
target_include_directories(RayLib PUBLIC ${STB_INCLUDE_DIRS})
target_link_libraries(RayLib PUBLIC fmt::fmt)

file(GLOB_RECURSE main_files main/*.cpp)
foreach(main_file ${main_files})
    get_filename_component(main_name ${main_file} NAME_WE)
    add_executable(${main_name} ${main_file})
    target_link_libraries(${main_name} PRIVATE RayLib)
endforeach()
