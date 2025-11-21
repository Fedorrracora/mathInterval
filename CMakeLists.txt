cmake_minimum_required(VERSION 3.18)
project(mathInterval LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TESTING "Enable testing" OFF)
option(CONSOLE "Enable console executable" OFF)

#set(PYBIND11_USE_ABI3 ON)
#set(PYBIND11_ABI_VERSION 3)

add_subdirectory(extern/pybind11)

if(CONSOLE)
    add_executable(mathIntervalConsole Testing/sample.cpp)
endif()

pybind11_add_module(mathInterval bindings.cpp)

if(TESTING)
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

    add_library(verify Testing/verifier.cpp)

    add_executable(test Testing/test1.cpp)

    target_link_libraries(test PRIVATE GTest::gtest_main)

#    add_executable(test_verify Testing/main_test.cpp)

#    target_link_libraries(test_verify verify)
endif()

install(TARGETS mathInterval
        LIBRARY DESTINATION mathInterval
        ARCHIVE DESTINATION mathInterval
        RUNTIME DESTINATION mathInterval)
