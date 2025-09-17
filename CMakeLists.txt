cmake_minimum_required(VERSION 3.5)
project(mathInterval LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(extern/pybind11)
add_executable(mathIntervalCpp
        sample.cpp)

pybind11_add_module(mathInterval bindings.cpp)

find_program(PYBIND11_STUBGEN_EXECUTABLE pybind11-stubgen)

if(PYBIND11_STUBGEN_EXECUTABLE)
    message(STATUS "Found pybind11-stubgen: ${PYBIND11_STUBGEN_EXECUTABLE}")

    add_custom_command(
        TARGET mathInterval
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR} ${PYBIND11_STUBGEN_EXECUTABLE} mathInterval
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating type stubs for mathInterval"
    )
else()
    message(WARNING "pybind11-stubgen not found. Skipping stub generation.")
endif()