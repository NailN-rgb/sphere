cmake_minimum_required(VERSION 3.0)

project(sphere)

find_package(Boost 1.66.0 REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

    message(${Boost_INCLUDE_DIRS})
    message(${Boost_LIBRARY_DIRS})
    message("using boost "
        ${Boost_MAJOR_VERSION} "."
        ${Boost_MINOR_VERSION} "."
        ${Boost_SUBMINOR_VERSION}
        " version"
    )
endif()

add_executable(sphere main.cpp)

target_link_libraries(sphere stdc++fs)
