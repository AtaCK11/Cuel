cmake_minimum_required(VERSION 3.29)
project(Cuel)

set(CMAKE_CXX_STANDARD 26)

add_executable(Cuel main.cpp
        tokenize.cpp
        tokenize.h
        parser.cpp
        parser.h)
