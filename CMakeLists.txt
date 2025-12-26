cmake_minimum_required(VERSION 3.18)
project(mathInterval LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(TESTING "Enable testing" ON)
option(BUILD_PYTHON_BINDINGS "Build Python bindings for development" OFF)
option(TESTING_BAR "Enable progress bar in testing" ON)
option(FORMATTER "Build tool for formatting test results to md. Using only in github workflows" OFF)
option(TEST_WRITER "Helps format c++ tests to python. Developers Only" OFF)

add_library(mathInterval INTERFACE)

target_include_directories(mathInterval INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# python
if (SKBUILD OR BUILD_PYTHON_BINDINGS)
    find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)

    if (TESTING AND NOT SKBUILD)
        execute_process(
                COMMAND ${Python3_EXECUTABLE} -c "import pytest"
                RESULT_VARIABLE EXIT_CODE
                OUTPUT_QUIET
                ERROR_QUIET
        )
        if (NOT EXIT_CODE STREQUAL "0")
            message(FATAL_ERROR "pytest is not installed. Install it using `pip install pytest` or disable testing.")
        endif ()
    endif ()

    add_subdirectory(extern/pybind11)

    pybind11_add_module(_mathInterval src/bindings.cpp)

    target_link_libraries(_mathInterval PRIVATE mathInterval)

    target_compile_definitions(_mathInterval PRIVATE INT_TYPE=long\ long FLOAT_TYPE=long\ double)
endif ()

if (TESTING AND NOT SKBUILD)
    #for google test on clang
    add_compile_options(-Wno-deprecated-declarations -Wcharacter-conversion)
    enable_testing()
    file(COPY Testing/one_set/answers DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/one_set)
    file(COPY Testing/custom_type/answers DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/custom_type)
    file(COPY Testing/many_set/answers DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/many_set)

    #setup GoogleTest
    include(FetchContent)
    if (POLICY CMP0135)
        cmake_policy(SET CMP0135 NEW)
    endif ()
    set(INSTALL_GTEST OFF CACHE BOOL "Disable installation of GoogleTest")
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)

    add_library(verify SHARED src/verifier.cpp src/additional_test_tools.cpp)
    target_include_directories(verify PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_compile_definitions(verify PRIVATE MAX_VALUE=1000000)

    add_executable(test_target
            Testing/one_set/cpp/test1.add_point.cpp
            Testing/one_set/cpp/test2.add_interval.cpp
            Testing/one_set/cpp/test3.multi_add.cpp
            Testing/one_set/cpp/test4.remove_clear_empty_full.cpp
            Testing/one_set/cpp/test5.inverse.cpp
            Testing/one_set/cpp/test6.in_inverse_points_only.cpp
            Testing/one_set/cpp/test7.transfer_operations.cpp
            Testing/one_set/cpp/test8.remainder.cpp
            Testing/one_set/cpp/test9.any.cpp
            Testing/many_set/cpp/test1.equal.cpp
            Testing/many_set/cpp/test2.subset.cpp
            Testing/custom_type/cpp/base.cpp
            Testing/custom_type/cpp/test1.many_funcs.cpp
            Testing/custom_type/cpp/test2.custom_transfer.cpp
            Testing/time_test/base.cpp
            Testing/time_test/test1.inverse.cpp
            Testing/time_test/test2.plus_transfer.cpp
            Testing/time_test/test3.mul_transfer.cpp
            Testing/time_test/test4.div_transfer.cpp
            Testing/generator/base.cpp
            Testing/generator/test1.inverse.cpp
            Testing/generator/test2.plus_transfer.cpp
            Testing/generator/test3.mul_transfer.cpp
            Testing/generator/test4.div_transfer.cpp
    )

    if (NOT TESTING_BAR)
        target_compile_definitions(test_target PRIVATE DISABLE_PROGRESS_BAR)
    endif ()
    target_link_libraries(test_target PRIVATE GTest::gtest_main PRIVATE mathInterval PRIVATE verify)
    target_compile_definitions(test_target PRIVATE RANDOM_GENERATOR=100000 CS=true WS=true BS=true ITERATIONS=1250)
    gtest_discover_tests(test_target WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    if (BUILD_PYTHON_BINDINGS)
        pybind11_add_module(verify_python src/verifier_bindings.cpp)
        target_link_libraries(verify_python PRIVATE verify)
        add_test(NAME PYTHON.ONE_SET.add_point COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/one_set/python/test1.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_test(NAME PYTHON.ONE_SET.add_interval COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/one_set/python/test2.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_test(NAME PYTHON.ONE_SET.multi_add COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/one_set/python/test3.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_test(NAME PYTHON.ONE_SET.remove_clear_empty_full COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/one_set/python/test4.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_test(NAME PYTHON.ONE_SET.inverse COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/one_set/python/test5.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_test(NAME PYTHON.ONE_SET.in_inverse_points_only COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/one_set/python/test6.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_test(NAME PYTHON.ONE_SET.transfer_operations COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/one_set/python/test7.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_test(NAME PYTHON.ONE_SET.remainder COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/one_set/python/test8.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_test(NAME PYTHON.ONE_SET.any COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/one_set/python/test9.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_test(NAME PYTHON.MANY_SET.equal COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/many_set/python/test1.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_test(NAME PYTHON.MANY_SET.subset COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/many_set/python/test2.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_test(NAME PYTHON.CUSTOM_TYPE.many_funcs COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/custom_type/python/test1.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_test(NAME PYTHON.CUSTOM_TYPE.custom_transfer COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_SOURCE_DIR}/Testing/custom_type/python/test2.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endif ()

    if (FORMATTER)
        target_compile_definitions(test_target PRIVATE FORMATTER_ENABLED)
        add_executable(formatter src/register.cpp Testing/time_test/time_test.register.cpp)
        target_link_libraries(formatter PRIVATE verify)
    endif ()

    if (TEST_WRITER)
        add_executable(writer src/writer.cpp)
        target_link_libraries(writer PRIVATE verify)
    endif ()
endif ()

# Python
if (SKBUILD)
    install(TARGETS _mathInterval
            LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME}
            ARCHIVE DESTINATION ${SKBUILD_PROJECT_NAME}
            RUNTIME DESTINATION ${SKBUILD_PROJECT_NAME}
    )
    install(FILES src/__init__.py DESTINATION ${SKBUILD_PROJECT_NAME})
    install(DIRECTORY src/_mathInterval/ DESTINATION ${SKBUILD_PROJECT_NAME}/_mathInterval OPTIONAL)

    # C++
else ()
    set(FINAL_MSG
            ""
            "=== building done ==="
            "-- For installing run: make install \\(Root privileges are required\\)"
    )
    set(FINAL_DEPS mathInterval)

    if (TESTING)
        list(APPEND FINAL_DEPS test_target verify gmock_main gmock)
        if (BUILD_PYTHON_BINDINGS)
            list(APPEND FINAL_DEPS _mathInterval)
        else ()
            list(APPEND FINAL_MSG
                    "-- Option BUILD_PYTHON_BINDINGS disabled. Python tests were not generated"
                    "Note that the BUILD_PYTHON_BINDINGS option is only used for testing. It cannot be used to install Python library in the system"
            )
        endif ()
        list(APPEND FINAL_MSG
                "-- For testing run: make test"
                "-- For C++ only testing run: ./test_target"
                "When you run ./test_target, you can see additional output"
        )
        if (NOT TESTING_BAR)
            list(APPEND FINAL_MSG
                    "-- Option TESTING_BAR disabled. Progress bar is disabled in all tests"
                    "This can be useful as the progress bar output breaks in some terminals"
            )
        endif ()
        if (FORMATTER)
            list(APPEND FINAL_DEPS formatter)
            list(APPEND FINAL_MSG
                    "-- Option FORMATTER enabled. Formatter executable built. This is only necessary for developers"
                    "It is used to worm the output of TIME_TESTS. Takes the path to the test output file and formats them into a markdown table"
                    "You can use it like this \\(in bash\\):"
                    "./test_target \\| tee ctest_output.txt"
                    "./formatter ctest_output.txt"
            )
        endif ()
        if (TEST_WRITER)
            list(APPEND FINAL_DEPS writer)
            list(APPEND FINAL_MSG
                    "-- Option TEST_WRITER enabled. Writer executable built. This is only necessary for developers"
                    "It helps converting simple c++ tests to python tests for mathInterval. The result often requires manual revisions"
                    "You can use it like this \\(in bash\\):"
                    "./writer test.cpp \\| tee test.py"
            )
        endif ()
    endif ()

    set(FINAL_CMD "")
    foreach (line IN LISTS FINAL_MSG)
        list(APPEND FINAL_CMD COMMAND ${CMAKE_COMMAND} -E echo "${line}")
    endforeach ()

    add_custom_target(final_message ALL ${FINAL_CMD})
    add_dependencies(final_message ${FINAL_DEPS})

    install(FILES include/interval.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mathInterval)
    install(TARGETS mathInterval EXPORT mathIntervalTargets)
    install(EXPORT mathIntervalTargets FILE mathIntervalTargets.cmake NAMESPACE mathInterval:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mathInterval)
    write_basic_package_version_file(mathIntervalConfigVersion.cmake VERSION 0.5.3 COMPATIBILITY AnyNewerVersion)
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/mathIntervalConfig.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/mathIntervalConfig.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mathInterval)
    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/mathIntervalConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/mathIntervalConfigVersion.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mathInterval)
endif ()