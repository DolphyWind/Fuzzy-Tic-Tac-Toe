cmake_minimum_required(VERSION 3.25)
project(fuzzy_tic_tac_toe)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BINARY_NAME ftt)

set(SOURCE_FILES
    src/main.cpp
)

add_executable(${BINARY_NAME} ${SOURCE_FILES})
target_include_directories(${BINARY_NAME} PRIVATE include)
