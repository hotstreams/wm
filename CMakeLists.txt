cmake_minimum_required(VERSION 3.19)
project(wm)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0 -ggdb -Wpedantic -Wextra -Wall -Wnon-virtual-dtor -Wunused)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
endif()

include_directories(include)

link_libraries(X11 GL)

add_executable(wm src/main.cpp src/wm/window_manager.cpp src/wm/window.cpp src/wm/display.cpp)