cmake_minimum_required(VERSION 3.0)

project(sphere)

find_package(Boost 1.66.0 REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(sphere main.cpp)