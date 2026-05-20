cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_CXX_STANDARD 17)
set(GLFW_BUILD_WAYLAND OFF)
set(GLFW_BUILD_X11 OFF)
set(GLFW_BUILD_DOCS OFF)
set(FASTNOISE2_TOOLS OFF)
set(FASTNOISE2_TESTS OFF)
set(FASTNOISE2_UTILITY OFF)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Release>:Release>)
endif ()

# I DO NOT REMEMBER [ANYONE] TELLING ME THAT ADD_SUBDIRECTORY MUST BE AFTER PROJECT

project(ererrea2)

add_subdirectory(libs/glad)
add_subdirectory(libs/glfw-3.4)
add_subdirectory(libs/stb)
add_subdirectory(libs/glm)
add_subdirectory(libs/FastNoise2)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${CMAKE_PROJECT_NAME} main.cpp ${MY_SOURCES})

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC 
    $<$<CONFIG:DEBUG>:RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/">
    $<$<NOT:$<CONFIG:DEBUG>>:RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/">
)

target_include_directories(ererrea2 PUBLIC include)
target_include_directories(ererrea2 PUBLIC src)
#FORCE STATIC LINKS, allowing exe in builddir to run
#https://stackoverflow.com/questions/38694058/cmake-linking-statically-against-libgcc-and-libstdc-into-a-shared-library
#add_library(myLib SHARED main.cpp ${MY_SOURCES})
set(CMAKE_EXE_LINKER_FLAGS " -static")
#target_link_libraries(myLib libgcc libstdc++)

target_link_libraries(ererrea2 PRIVATE glad glfw stb_image glm FastNoise2)

project(oldmain)
add_executable(${CMAKE_PROJECT_NAME} oldmain.cpp)
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
        $<$<CONFIG:DEBUG>:RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/">
        $<$<NOT:$<CONFIG:DEBUG>>:RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/">
)
target_include_directories(oldmain PUBLIC include)
target_include_directories(oldmain PUBLIC src)
set(CMAKE_EXE_LINKER_FLAGS " -static")
target_link_libraries(oldmain PRIVATE glad glfw stb_image glm)