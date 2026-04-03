cmake_minimum_required(VERSION 3.25)
project(fuzzy_tic_tac_toe)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BINARY_NAME fttt)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(SOURCE_FILES
    src/main.cpp
    src/Cell.cpp
    src/FTTTBase.cpp
    src/FTTTBoard.cpp
    src/FTTTGame.cpp
)

find_package(GMPXX REQUIRED)

add_executable(${BINARY_NAME} ${SOURCE_FILES})
target_include_directories(${BINARY_NAME} PRIVATE include ${GMPXX_INCLUDE_DIRS})
target_link_libraries(${BINARY_NAME} PRIVATE ${GMPXX_LIBRARIES})
