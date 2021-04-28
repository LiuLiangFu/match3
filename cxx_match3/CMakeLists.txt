cmake_minimum_required(VERSION 3.15)
project(cxx_match3)

add_subdirectory(pybind11)
# find_package(pybind11 REQUIRED)

add_library(game OBJECT Game.cpp)
add_library(board OBJECT Board.cpp)
add_library(filler OBJECT Filler.cpp)
add_library(searcher OBJECT Searcher.cpp)

pybind11_add_module(
    module_name
    module.cpp
    $<TARGET_OBJECTS:game>
    $<TARGET_OBJECTS:board>
    $<TARGET_OBJECTS:filler>
    $<TARGET_OBJECTS:searcher>
)
# target_link_libraries(module_name PRIVATE $<TARGET_OBJECTS:game>)

set(CMAKE_CXX_STANDARD 17)

add_executable(cxx_match3 main.cpp Board.cpp Board.h Filler.cpp Filler.h Game.cpp Game.h Searcher.cpp Searcher.h)

