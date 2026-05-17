cmake_minimum_required(VERSION 3.25)
project(fuzzy_tic_tac_toe)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BINARY_NAME fttt)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(ENABLE_PYTHON "Enable python target. Requires python and boost.python to be installed." OFF)

set(SOURCE_FILES
    src/main.cpp
    src/Cell.cpp
    src/FTTTBase.cpp
    src/FTTTGame.cpp
)

find_package(GMPXX REQUIRED)

if(ENABLE_PYTHON)
    find_package(Boost REQUIRED COMPONENTS python)
    find_package(Python3 REQUIRED COMPONENTS Development)
    set(LIB_NAME libfttt)
    add_library(${LIB_NAME} SHARED ${SOURCE_FILES} src/python_export.cpp)
    set_target_properties(${LIB_NAME} PROPERTIES PREFIX "")
    set_property(TARGET ${LIB_NAME} PROPERTY OUTPUT_NAME ${BINARY_NAME})
    target_include_directories(${LIB_NAME} PRIVATE include ${GMPXX_INCLUDE_DIRS})
    target_link_libraries(${LIB_NAME} PRIVATE ${GMPXX_LIBRARIES} Boost::python Python3::Python)
endif()

add_executable(${BINARY_NAME} ${SOURCE_FILES})
target_include_directories(${BINARY_NAME} PRIVATE include ${GMPXX_INCLUDE_DIRS})
target_link_libraries(${BINARY_NAME} PRIVATE ${GMPXX_LIBRARIES})
