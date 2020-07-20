# Copyright (C) 2020 - Roman Byshliaha

cmake_minimum_required(VERSION 3.10)

project(interval_map LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fstack-protector-strong -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "-g0")
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG} --coverage")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE "include")

option(BUILD_TESTS "Build tests" OFF)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif (BUILD_TESTS)
