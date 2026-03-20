# THIS WAS NOT AUTO GENERATED
# Remember to copy this file into every project using ....
cmake_minimum_required(VERSION 4.0)

add_library(glfw-3.5)
target_sources(glfw-3.5 PRIVATE lib-mingw-w64/libglfw3.a)
target_sources(glfw-3.5 PRIVATE lib-mingw-w64/glfw3.dll)
target_link_libraries(glfw-3.5 PRIVATE lib-mingw-w64/libglfw3.a)
target_link_directories(glfw-3.5 PUBLIC include)