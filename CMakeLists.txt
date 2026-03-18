cmake_minimum_required(VERSION 4.0)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Release>:Release>)
endif ()

#add_subdirectory(thirdParty/alib)

project(ererrea2)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${CMAKE_PROJECT_NAME} main.cpp ${MY_SOURCES})

#target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC RESOURCES_PATH=${CMAKE_CURRENT_SOURCE_DIR}/resources/)
#target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC RESOURCES_PATH=.resources/)

target_include_directories(ererrea2 PUBLIC include)
#FORCE STATIC LINKS, allowing exe in builddir to run
#https://stackoverflow.com/questions/38694058/cmake-linking-statically-against-libgcc-and-libstdc-into-a-shared-library
add_library(myLib SHARED main.cpp ${MY_SOURCES})
set(CMAKE_EXE_LINKER_FLAGS " -static")
target_link_libraries(myLib libgcc libstdc++)
#target_link_libraries(ererrea2 PRIVATE alib)