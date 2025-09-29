//
// Created by fedor on 17.09.2025.
//
#include <format>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
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
        .def("__str__", &Interval::print, "return string with all data in mathematical style")
        .def("__contains__", static_cast<bool (Interval::*)(const typename Interval::inp_type&) const>(&Interval::in), "return true if this point in multitude, else return false")
        .def("__contains__", &Interval::issubset, "return true if this multitude is subset of another multitude, else return false")
        .def("contains", static_cast<bool (Interval::*)(const typename Interval::inp_type&, const typename Interval::inp_type&) const>(&Interval::in), "return true if interval (a, b) in multitude, else return false")
        .def("issubset", &Interval::issubset, "return true if this multitude is subset of another multitude, else return false")
        .def("issuperset", &Interval::issuperset, "return true if this multitude is subset of another multitude, else return false")
        .def("isdisjoint", &Interval::isdisjoint, "return true if these multitudes has no common points, else return false")
        .def("add_interval", &Interval::add_interval, "returns false if all this interval was inside this multitude, else return true")
        .def("add_point", &Interval::add_point, "returns false if this point was inside this multitude, else return true")
        .def("remove_interval", &Interval::remove_interval, "returns false if all this interval was not inside this multitude, else return true")
        .def("remove_point", &Interval::remove_point, "returns false if this point was not inside this multitude, else return true")
        .def("empty", &Interval::empty, "return true if this multitude is empty, else return false")
        .def("points_only", &Interval::points_only, "return true if multitude has only separate points (or empty), else return false")
        .def("clear", &Interval::clear, "clear multitude data")
        .def("inverse", &Interval::invert, "returns the multitude that is the inverse of the given one")
        .def("__add__", [](const Interval &a, const Interval &b) {return a + b;}, py::is_operator(), "returns a new multitude containing the union of the elements of the previous multitudes")
        .def("__or__", [](const Interval &a, const Interval &b) {return a + b;}, py::is_operator(), "returns a new multitude containing the union of the elements of the previous multitudes")
        .def("__iadd__", [](Interval &a, const Interval &b) {return a += b;}, py::is_operator(), "adds elements of another multitude")
        .def("__ior__", [](Interval &a, const Interval &b) {return a += b;}, py::is_operator(), "adds elements of another multitude")
        .def("__sub__", [](const Interval &a, const Interval &b) {return a - b;}, py::is_operator(), "returns a new multitude containing the difference of the elements of the previous multitudes")
        .def("__isub__", [](Interval &a, const Interval &b) {return a -= b;}, py::is_operator(), "remove elements of another multitude")
        .def("__mul__", [](const Interval &a, const Interval &b) {return a * b;}, py::is_operator(), "returns a new multitude containing the intersection of the elements of the previous multitudes")
        .def("__and__", [](const Interval &a, const Interval &b) {return a * b;}, py::is_operator(), "returns a new multitude containing the intersection of the elements of the previous multitudes")
        .def("__imul__", [](Interval &a, const Interval &b) {return a *= b;}, py::is_operator(), "intersect elements with another multitude")
        .def("__iand__", [](Interval &a, const Interval &b) {return a *= b;}, py::is_operator(), "intersect elements with another multitude")
        .def("__xor__", [](const Interval &a, const Interval &b) {return a ^ b;}, py::is_operator(), "returns a new multitude containing the symmetric difference of the elements of the previous multitudes")
        .def("__ixor__", [](Interval &a, const Interval &b) {return a ^= b;}, py::is_operator(), " generating symmetric difference with elements of another multitude")
        .def("any", [](const Interval &a) {return a.any(true);}, R"doc(
### any

Return any element that is in data.

The function can return `None`, because the returning value does not always exist.

---

- If there is any point, it will be returned.
- If there is an interval `(-INF; +INF)`, the function will return `None`.
- If it is `mathInterval_int` or `mathInterval_float`,
  a smart algorithm will try to find any number in the intervals.
- If it is `mathInterval_str`,
  a smart algorithm will try to find any string in the intervals,
  considering that a string may contain only **capital English letters**.
- If it is standard `mathInterval`, or if the algorithm does not find any element in data,
  the function will return `None`.

---

For custom types and algorithms, consider using this function with additional arguments.
)doc")
        .def("any", static_cast<std::optional<T> (Interval::*)(
                const std::function<std::optional<T>(const T&)> &,
                const std::function<std::optional<T>(const T&)> &,
                const std::function<std::optional<T>(const T&, const T&)> &, T) const>(&Interval::any), R"doc(
### any()

Return any element that is in data.

The function can return `None`, because the returning value does not always exist.

This function takes **three lambda functions** and **one value**:

---

- **First lambda** – called if there is an interval `(-INF; x)`,
  receives one argument (x).
- **Second lambda** – called if there is an interval `(x; +INF)`,
  receives one argument (x).
- **Third lambda** – called if there is an interval `(x; y)`,
  receives two arguments (x, y).
- **Value** - result for interval `(-INF, +INF)`

---

A lambdas may return `None`, if the interval has no integer value.

⚠️ **Warning:**
You must yourself detect that the returning value lies inside the interval.
)doc")
        .def("custom_transfer", static_cast<Interval (Interval::*)(const std::function<T(const T&)> &) const>(&Interval::custom_transfer), R"doc(
### custom_transfer()

Transfer all elements that are in this multitude and return a new multitude.

The function takes **one lambda function**.
`-INF` and `+INF` remain unchanged.

---

- **Lambda** – returns a new value of a point/border of an interval,
  receives one argument - old value of a point/border of an interval.

---

If the first value of an interval becomes greater than the second,
the function will swap them automatically.
)doc")
        .def("custom_transfer", static_cast<Interval (Interval::*)
                (const std::function<T(const T&)> &, const T&, const T&) const>(&Interval::custom_transfer), R"doc(
### custom_transfer()

Transfer all elements that are in this multitude and return a new multitude.

The function takes **one lambda function**
and **two values** – the converted values of `-INF` and `+INF`.
New values cannot themselves be `-INF` or `+INF`.

---

- **Lambda** – returns a new value of a point/border of an interval,
  receives one argument - old value of a point/border of an interval.
- **First value** – new value of the border of the interval, that begins from `-INF` - old value of a border of an interval.
- **Second value** – new value of the border of the interval, that ends with `+INF` - old value of a border of an interval.

---

If the first value of an interval becomes greater than the second,
the function will swap them automatically.
)doc")
    ;
    if constexpr (std::is_arithmetic_v<T>) cls
        .def("__add__", [](const Interval &a, const T &b) {return a + b;}, py::is_operator(), "returns a new multitude with the points shifted forward by the distance val")
        .def("__iadd__", [](Interval &a, const T &b) {return a += b;}, py::is_operator(), "shift the points forward by a distance of val")
        .def("__sub__", [](const Interval &a, const T &b) {return a - b;}, py::is_operator(), "returns a new multitude with the points shifted backward by the distance val")
        .def("__isub__", [](Interval &a, const T &b) {return a -= b;}, py::is_operator(), "shift the points backward by a distance of val")
        .def("__mul__", [](const Interval &a, const T &b) {return a * b;}, py::is_operator(), "returns a new multitude with the points multiplied by a factor of val")
        .def("__imul__", [](Interval &a, const T &b) {return a *= b;}, py::is_operator(), "multiplies the points of a multitude by a factor of val")
    ;
    if constexpr (std::is_integral_v<T>) cls
        .def("__floordiv__", [](const Interval &a, const T &b) {return a / b;}, py::is_operator(), "returns a new multitude with the points divided by a factor of val")
        .def("__ifloordiv__", [](Interval &a, const T &b) {return a /= b;}, py::is_operator(), "divides the points of a multitude by a factor of val")
        .def("__mod__", [](const Interval &a, const T &b) {return a % b;}, py::is_operator(), "returns a new multitude with points taken as the remainder of the division by val")
        .def("__imod__", [](Interval &a, const T &b) {return a %= b;}, py::is_operator(), "replaces the points with the remainder of the division by val")
    ;
    if constexpr (std::is_arithmetic_v<T> && !std::is_integral_v<T>) cls
        .def("__truediv__", [](const Interval &a, const T &b) {return a / b;}, py::is_operator(), "returns a new multitude with the points divided by a factor of val")
        .def("__itruediv__", [](Interval &a, const T &b) {return a /= b;}, py::is_operator(), "divides the points of a multitude by a factor of val")
    ;
    cls.def_property_readonly_static("minimal", [](const py::object&) { return Min(); }, "return always minimal element in any interval");
    cls.def_property_readonly_static("maximal", [](const py::object&) { return Max(); }, "return always maximal element in any interval");
}

PYBIND11_MODULE(mathInterval, m) {
    m.doc() = R"doc(
mathInterval
============

C++ interval arithmetic exposed to Python.

This module provides classes and algorithms for working
with mathematical multitudes. It supports:

- Construction of multitudes with finite or infinite bounds.
- Smart search algorithms using user-provided lambdas.
- Conversion and custom transfer of interval data.
- Executing multiple operators between multitudes.

All classes and functions are documented with Python-style docstrings.
)doc";
    bind_abc<long long>(m, "Interval_int");
    bind_abc<long double>(m, "Interval_float");
    bind_abc<std::string>(m, "Interval_str");
    bind_abc<py::object>(m, "Interval");
}