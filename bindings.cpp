//
// Created by fedor on 17.09.2025.
//
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "interval.h"
namespace py = pybind11;
template<typename T>
void bind_abc(py::module_ &m, const char* class_name) {
    py::class_<interval::interval<T>>(m, class_name)
        .def(py::init<>())
        .def("add_interval", &interval::interval<T>::add_interval)
        .def("print", &interval::interval<T>::print)
        .def("add_point", &interval::interval<T>::add_point);
}

PYBIND11_MODULE(mathInterval, m) {
    m.doc() = "Python bindings for mathInterval<T>";
    bind_abc<int>(m, "abc_int");
    bind_abc<double>(m, "abc_double");
    bind_abc<std::string>(m, "abc_str");
}