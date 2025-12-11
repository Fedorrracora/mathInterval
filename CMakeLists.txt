cmake_minimum_required(VERSION 3.18)
project(mathInterval LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(TESTING "Enable testing" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python bindings for development" OFF)

add_library(mathInterval INTERFACE)

target_include_directories(mathInterval INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# python
if(SKBUILD OR BUILD_PYTHON_BINDINGS)
    #set(PYBIND11_USE_ABI3 ON)
    #set(PYBIND11_ABI_VERSION 3)
    find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)

    if(TESTING)
        execute_process(
                COMMAND ${Python3_EXECUTABLE} -c "import pytest"
                RESULT_VARIABLE EXIT_CODE
                OUTPUT_QUIET
                ERROR_QUIET
        )
        if (NOT EXIT_CODE STREQUAL "0")
            message(FATAL_ERROR "pytest is not installed. Install it using `pip install pytest` or disable testing.")
        endif()
    endif()

    add_subdirectory(extern/pybind11)

    pybind11_add_module(_mathInterval src/bindings.cpp)

    target_link_libraries(_mathInterval PRIVATE mathInterval)

    target_compile_definitions(_mathInterval PRIVATE INT_TYPE=long\ long FLOAT_TYPE=long\ double)
endif()

if(TESTING AND NOT SKBUILD)
    enable_testing()
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Testing/answers DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    #setup GoogleTest
    include(FetchContent)
    if(POLICY CMP0135)
        cmake_policy(SET CMP0135 NEW)
    endif()
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)

    add_library(verify SHARED src/verifier.cpp)
    target_include_directories(verify PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    add_library(additional_test_tools STATIC src/additional_test_tools.cpp)
    target_include_directories(additional_test_tools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_compile_definitions(additional_test_tools PRIVATE MAX_VALUE=1000000)

    add_executable(test_target
            Testing/cpp/test1.cpp
            Testing/cpp/test2.cpp
            Testing/cpp/test3.cpp
            Testing/cpp/test4.cpp
            Testing/cpp/test5.cpp
            Testing/cpp/test6.cpp
            Testing/cpp/test7.cpp
            Testing/cpp/time_test1.cpp
            Testing/cpp/test_generator1.cpp)
    target_link_libraries(test_target PRIVATE GTest::gtest_main PRIVATE verify PRIVATE additional_test_tools)
    target_compile_options(test_target PRIVATE -Wno-deprecated-declarations)
    target_compile_definitions(test_target PRIVATE ITERATIONS=2500 RANDOM_GENERATOR=100000)
    gtest_discover_tests(test_target WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    if(BUILD_PYTHON_BINDINGS)
        pybind11_add_module(verify_python src/verifier_bindings.cpp)
        target_link_libraries(verify_python PRIVATE verify)
        add_test(NAME PYTHON.ONE_SET.add_point COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/python/test1.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_test(NAME PYTHON.ONE_SET.add_interval COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/python/test2.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endif()
endif()

# Python
if(SKBUILD)
    install(TARGETS _mathInterval
            LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME}
            ARCHIVE DESTINATION ${SKBUILD_PROJECT_NAME}
            RUNTIME DESTINATION ${SKBUILD_PROJECT_NAME}
    )
    install(FILES src/__init__.py DESTINATION ${SKBUILD_PROJECT_NAME})
    install(DIRECTORY src/_mathInterval/ DESTINATION ${SKBUILD_PROJECT_NAME}/_mathInterval OPTIONAL)

# C++
else()
    if(TESTING)
        add_custom_target(final_message ALL
                COMMAND ${CMAKE_COMMAND} -E echo ""
                COMMAND ${CMAKE_COMMAND} -E echo "For installing run: sudo make install"
                COMMAND ${CMAKE_COMMAND} -E echo "For testing run: make test"
                COMMAND ${CMAKE_COMMAND} -E echo "For C++ only testing run: ./test_target"
                COMMAND ${CMAKE_COMMAND} -E echo "when you run ./test_target, you can see additional output"
        )
        add_dependencies(final_message mathInterval test_target gmock_main gmock)
    else()
        add_custom_target(final_message ALL
                COMMAND ${CMAKE_COMMAND} -E echo ""
                COMMAND ${CMAKE_COMMAND} -E echo "=== building done ==="
                COMMAND ${CMAKE_COMMAND} -E echo "Now run: sudo make install"
        )
        add_dependencies(final_message mathInterval)
    endif ()
    install(FILES include/interval.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mathInterval)
    install(TARGETS mathInterval EXPORT mathIntervalTargets)
    install(EXPORT mathIntervalTargets FILE mathIntervalTargets.cmake NAMESPACE mathInterval:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mathInterval)
    write_basic_package_version_file(mathIntervalConfigVersion.cmake VERSION 0.4.1 COMPATIBILITY AnyNewerVersion)
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/mathIntervalConfig.cmake.in
                                  ${CMAKE_CURRENT_BINARY_DIR}/mathIntervalConfig.cmake
                                  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mathInterval)
    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/mathIntervalConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/mathIntervalConfigVersion.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mathInterval)
endif()