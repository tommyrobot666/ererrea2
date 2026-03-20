# THIS WAS NOT AUTO GENERATED
# Remember to copy this file into every project using glad
cmake_minimum_required(VERSION 4.0)

add_library(glad)
target_sources(glad PRIVATE src/glad.c)
target_link_directories(glad PUBLIC include)