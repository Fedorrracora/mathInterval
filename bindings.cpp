//
// Created by fedor on 17.09.2025.
//
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "interval.h"
namespace py = pybind11;
template<typename T>
void bind_abc(py::module_ &m, const char* class_name) {
    using Interval = interval::interval<T>;
    using Min = interval::minimal<T>;
    using Max = interval::maximal<T>;

    auto min_cls = py::class_<Min>(m, std::string("_").append(class_name).append("_minimal").c_str())
        .def(py::init<>())
        .def("__repr__", [](const Min&) { return "<minimal>"; });

    auto max_cls = py::class_<Max>(m, std::string("_").append(class_name).append("_maximal").c_str())
        .def(py::init<>())
        .def("__repr__", [](const Max&) { return "<maximal>"; });

    py::class_<Interval> cls(m, class_name);
    cls.def(py::init<>())
        .def("__str__", &Interval::print)
        .def("__contains__", static_cast<bool (Interval::*)(const typename Interval::inp_type&) const>(&Interval::in))
        .def("contains", static_cast<bool (Interval::*)(const typename Interval::inp_type&, const typename Interval::inp_type&) const>(&Interval::in))
        .def("add_interval", &Interval::add_interval)
        .def("add_point", &Interval::add_point)
        .def("remove_interval", &Interval::remove_interval)
        .def("remove_point", &Interval::remove_point)
        .def("empty", &Interval::empty)
        .def("clear", &Interval::clear)
        .def("inverse", &Interval::invert)
        .def("__add__", [](const Interval &a, const Interval &b) {return a + b;}, py::is_operator())
        .def("__iadd__", [](Interval &a, const Interval &b) {return a += b;}, py::is_operator())
        .def("__sub__", [](const Interval &a, const Interval &b) {return a - b;}, py::is_operator())
        .def("__isub__", [](Interval &a, const Interval &b) {return a -= b;}, py::is_operator())
        .def("__mul__", [](const Interval &a, const Interval &b) {return a * b;}, py::is_operator())
        .def("__imul__", [](Interval &a, const Interval &b) {return a *= b;}, py::is_operator())
    ;
    if constexpr (std::is_arithmetic_v<T>) cls
        .def("__add__", [](const Interval &a, const T &b) {return a + b;}, py::is_operator())
        .def("__iadd__", [](Interval &a, const T &b) {return a += b;}, py::is_operator())
        .def("__sub__", [](const Interval &a, const T &b) {return a - b;}, py::is_operator())
        .def("__isub__", [](Interval &a, const T &b) {return a -= b;}, py::is_operator())
        .def("__mul__", [](const Interval &a, const T &b) {return a * b;}, py::is_operator())
        .def("__imul__", [](Interval &a, const T &b) {return a *= b;}, py::is_operator())
    ;
    if constexpr (std::is_integral_v<T>) cls
        .def("__floordiv__", [](const Interval &a, const T &b) {return a / b;}, py::is_operator())
        .def("__ifloordiv__", [](Interval &a, const T &b) {return a /= b;}, py::is_operator())
    ;
    if constexpr (std::is_arithmetic_v<T> && !std::is_integral_v<T>) cls
        .def("__truediv__", [](const Interval &a, const T &b) {return a / b;}, py::is_operator())
        .def("__itruediv__", [](Interval &a, const T &b) {return a /= b;}, py::is_operator())
    ;
    cls.def_property_readonly_static("minimal", [](const py::object&) { return Min(); });
    cls.def_property_readonly_static("maximal", [](const py::object&) { return Max(); });
}

PYBIND11_MODULE(mathInterval, m) {
    m.doc() = "Python bindings for mathInterval<T>";
    bind_abc<long long>(m, "mathInterval_int");
    bind_abc<long double>(m, "mathInterval_float");
    bind_abc<std::string>(m, "mathInterval_str");
    bind_abc<py::object>(m, "mathInterval");
}