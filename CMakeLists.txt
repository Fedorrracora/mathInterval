cmake_minimum_required(VERSION 3.18)
project(mathInterval LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PYBIND11_USE_ABI3 ON)
set(PYBIND11_ABI_VERSION 3)

add_subdirectory(extern/pybind11)

# Сама библиотека
pybind11_add_module(mathInterval bindings.cpp)

# >>> Ключевой момент <<<
# Устанавливаем именно в папку Python-пакета
install(TARGETS mathInterval
        LIBRARY DESTINATION mathInterval     # <-- имя каталога Python-пакета
        ARCHIVE DESTINATION mathInterval
        RUNTIME DESTINATION mathInterval)
