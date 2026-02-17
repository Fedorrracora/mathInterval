cmake_minimum_required(VERSION 3.18)
project(fbp-forward-container VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

option(FBP_INSTALL "Enable installation of FBP libraries" ON)
option(FBP_TESTING "Enable testing of FBP libraries" ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(forward_container INTERFACE)
add_library(fbp::forward_container ALIAS forward_container)

target_include_directories(forward_container INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(FBP_TESTING)
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

    add_executable(forward_container_test_target Testing/test.cpp)
    target_link_libraries(forward_container_test_target PRIVATE GTest::gtest_main PRIVATE forward_container)
    set_target_properties(forward_container_test_target PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Tests)
    gtest_discover_tests(forward_container_test_target)
endif()

if(FBP_INSTALL)
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(TARGETS forward_container EXPORT forward_containerTargets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    write_basic_package_version_file(
            ${CMAKE_CURRENT_BINARY_DIR}/fbp-forward_containerConfigVersion.cmake
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY AnyNewerVersion
    )

    configure_package_config_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/fbp-forward_containerConfig.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/fbp-forward_containerConfig.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fbp-forward_container
    )

    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/fbp-forward_containerConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/fbp-forward_containerConfigVersion.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fbp-forward_container
    )

    install(EXPORT forward_containerTargets
            FILE fbp-forward_containerTargets.cmake
            NAMESPACE fbp::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fbp-forward_container
    )
endif()