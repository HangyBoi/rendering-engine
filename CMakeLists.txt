cmake_minimum_required(VERSION 3.20)
project(RenderingEngines LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Every .cpp file directly inside a bootcamp-XX folder becomes its own program.
# Example: bootcamp-01/ex12_divisors.cpp becomes the program "bootcamp-01_ex12_divisors".
file(GLOB EXERCISES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/bootcamp-*/*.cpp")

foreach(SOURCE_FILE ${EXERCISES})
    get_filename_component(FOLDER_PATH ${SOURCE_FILE} DIRECTORY)
    get_filename_component(FOLDER_NAME ${FOLDER_PATH} NAME)
    get_filename_component(EXERCISE_NAME ${SOURCE_FILE} NAME_WE)
    add_executable(${FOLDER_NAME}_${EXERCISE_NAME} ${SOURCE_FILE})
endforeach()