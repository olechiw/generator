cmake_minimum_required(VERSION 3.25)
project(generator)

set(CMAKE_CXX_STANDARD 23)

add_executable(generator main.cpp
        TraitGenerator.cpp
        TraitGenerator.h
        RandomSelectionTree.h)
