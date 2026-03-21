# THIS WAS NOT AUTO GENERATED
# Remember to copy this file into every project using glad
cmake_minimum_required(VERSION 3.28.3)
project(glad)

add_library(glad)
target_sources(glad PRIVATE src/glad.c)
#target_link_directories(glad PUBLIC include) FOR COMPILED FILES (IT LINKS THE PRECOMPILED STUFF)
target_include_directories(glad PUBLIC include)