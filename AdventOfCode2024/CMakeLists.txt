cmake_minimum_required(VERSION 3.25)

project(AdventOfCode2024 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

file(GLOB_RECURSE CPP_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
message(STATUS "Discovered source files: ${CPP_SOURCES}")

foreach (SOURCE_FILE ${CPP_SOURCES})
    get_filename_component(EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE})
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    set(DATA_DIR "${CMAKE_SOURCE_DIR}/data/${EXECUTABLE_NAME}")

    add_custom_target(run_${EXECUTABLE_NAME}_question
            COMMAND ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME} ${DATA_DIR}/question.input
            DEPENDS ${EXECUTABLE_NAME}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            COMMENT "Running ${EXECUTABLE_NAME} with ${DATA_DIR}/question.input")

    add_custom_target(run_${EXECUTABLE_NAME}_test
            COMMAND ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME} ${DATA_DIR}/test.input
            DEPENDS ${EXECUTABLE_NAME}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            COMMENT "Running ${EXECUTABLE_NAME} with ${DATA_DIR}/test.input")

    message(STATUS "Added executable: ${EXECUTABLE_NAME}")
endforeach ()




