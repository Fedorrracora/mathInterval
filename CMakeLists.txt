cmake_minimum_required(VERSION 3.18)
project(mathInterval LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)

option(TESTING "Enable testing" OFF)
option(CONSOLE "Enable console executable" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python bindings for development" OFF)

add_library(mathInterval INTERFACE)

target_include_directories(mathInterval INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(CONSOLE)
    add_executable(mathIntervalConsole test/sample.cpp)
endif()

# python
if(SKBUILD OR BUILD_PYTHON_BINDINGS)
    #set(PYBIND11_USE_ABI3 ON)
    #set(PYBIND11_ABI_VERSION 3)
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

    add_library(verify STATIC src/verifier.cpp)
    target_include_directories(verify PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    add_executable(test_target
            Testing/cpp/test1.cpp
            Testing/cpp/test2.cpp
            Testing/cpp/test3.cpp
            Testing/cpp/test4.cpp) # todo: add tests
    target_link_libraries(test_target PRIVATE GTest::gtest_main PRIVATE verify)
    gtest_discover_tests(test_target WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_executable(test_verify Testing/main_test.cpp) # todo: remove
    target_link_libraries(test_verify verify)
endif()

# Python
if(SKBUILD)
install(TARGETS _mathInterval
        LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME}
        ARCHIVE DESTINATION ${SKBUILD_PROJECT_NAME}
        RUNTIME DESTINATION ${SKBUILD_PROJECT_NAME}
)
install(FILES src/__init__.py DESTINATION ${SKBUILD_PROJECT_NAME})

# C++
else()
install(FILES include/interval.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mathInterval)
install(TARGETS mathInterval EXPORT mathIntervalTargets)
install(EXPORT mathIntervalTargets FILE mathIntervalTargets.cmake NAMESPACE mathInterval:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mathInterval)
write_basic_package_version_file(mathIntervalConfigVersion.cmake VERSION 0.4.0 COMPATIBILITY AnyNewerVersion)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/mathIntervalConfig.cmake.in
                              ${CMAKE_CURRENT_BINARY_DIR}/mathIntervalConfig.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mathInterval)
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/mathIntervalConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/mathIntervalConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mathInterval)
endif()