cmake_minimum_required(VERSION 3.15)
project(cxx_match3)

set(CMAKE_CXX_STANDARD 17)

add_executable(cxx_match3 main.cpp Board.cpp Board.h Filler.cpp Filler.h Game.cpp Game.h Searcher.cpp Searcher.h)