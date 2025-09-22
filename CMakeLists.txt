cmake_minimum_required(VERSION 3.18)
project(mathInterval LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PYBIND11_USE_ABI3 ON)
set(PYBIND11_ABI_VERSION 3)

add_subdirectory(extern/pybind11)


add_executable(mathIntervalCpp sample.cpp)

pybind11_add_module(mathInterval bindings.cpp)

find_program(PYBIND11_STUBGEN_EXECUTABLE pybind11-stubgen)

set(PY_BUILD "${CMAKE_BINARY_DIR}/py_build")

if(PYBIND11_STUBGEN_EXECUTABLE)
    message(STATUS "Found pybind11-stubgen: ${PYBIND11_STUBGEN_EXECUTABLE}")

    add_custom_target(package_py ALL
        COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR} ${PYBIND11_STUBGEN_EXECUTABLE} mathInterval
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS mathInterval
        COMMENT "Generating type stubs for mathInterval"
    )

    add_custom_command(
        TARGET package_py POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
                ${PY_BUILD}/mathInterval/mathInterval
        COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_FILE:mathInterval>
                ${PY_BUILD}/mathInterval/mathInterval/mathInterval.so
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/stubs/mathInterval.pyi
                ${PY_BUILD}/mathInterval/mathInterval/mathInterval.pyi
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/base/__init__.py
                ${PY_BUILD}/mathInterval/mathInterval/__init__.py
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/base/setup.py
                ${PY_BUILD}/mathInterval/setup.py
        COMMENT "Packing Python-module in ${PY_BUILD}"
    )
else()
    message(WARNING "pybind11-stubgen not found. Skipping stub generation (highlighting in IDEs will not work).")
    add_custom_command(
        TARGET mathInterval POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
                ${PY_BUILD}/mathInterval/mathInterval
        COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_FILE:mathInterval>
                ${PY_BUILD}/mathInterval/mathInterval/mathInterval.so
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/base/__init__.py
                ${PY_BUILD}/mathInterval/mathInterval/__init__.py
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/base/setup_without_stubgen.py
                ${PY_BUILD}/mathInterval/setup.py
        COMMENT "Packing Python-module in ${PY_BUILD}"
    )
endif()

function(check_py)
    execute_process(
        COMMAND ${PIP_EXEC} --version
        RESULT_VARIABLE pip_check_result
    )
    if(NOT pip_check_result EQUAL 0)
        set(PIP_AVAILABLE FALSE PARENT_SCOPE)
    else()
        set(PIP_AVAILABLE TRUE PARENT_SCOPE)
    endif()
endfunction()

message(STATUS "checking pip")
set(PIP_EXEC "python;-m;pip")

check_py()

if(NOT PIP_AVAILABLE)
    message(WARNING "python -m pip not work. Trying to use \`pip\`")
    set(PIP_EXEC "pip")
    check_py()
    if(NOT PIP_AVAILABLE)
        message(FATAL_ERROR "pip don`t available.")
    endif()
endif()

message(STATUS "Installing Python package")

# ---- make install ----
install(CODE "
    message(STATUS \"Installing Python package\")
    execute_process(
        COMMAND ${PIP_EXEC} install .
        WORKING_DIRECTORY ${PY_BUILD}/mathInterval
    )
")

# ---- make uninstall ----
add_custom_target(uninstall
    COMMAND ${PIP_EXEC} uninstall -y mathInterval
    WORKING_DIRECTORY ${PY_BUILD}/mathInterval
    COMMENT "️ Uninstalling Python package mathInterval"
)