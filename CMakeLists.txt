cmake_minimum_required(VERSION 4.2)

project(
    syclope
    DESCRIPTION "Leetcode exercises"
    LANGUAGES CXX
    VERSION 0.0.1
)

include(cmake/flags.cmake)

add_subdirectory(extern)

add_subdirectory(app)
