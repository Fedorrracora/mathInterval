#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <interval.h>
namespace py = pybind11;
template<typename T, typename type_policy> requires std::is_base_of_v<interval::detail::type_policy, type_policy>
void bind_interval(py::module_ &m, const char* class_name) {
    using Interval = interval::interval<T, type_policy>;
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
        .def("__str__", &Interval::to_string, "return string with all data in mathematical style")
        .def("__contains__", &Interval::issubset, py::arg("a"), "return true if this multitude is subset of another multitude, else return false")
        .def("__contains__", static_cast<bool (Interval::*)(const typename Interval::inp_type&) const>(&Interval::in), py::arg("a"), "return true if this point in multitude, else return false")
        .def("contains", static_cast<bool (Interval::*)(const typename Interval::inp_type&, const typename Interval::inp_type&) const>(&Interval::in), py::arg("a"), py::arg("b"), "return true if interval (a, b) in multitude, else return false")
        .def("issubset", &Interval::issubset, py::arg("b"), "return true if this multitude is subset of another multitude, else return false")
        .def("issuperset", &Interval::issuperset, py::arg("b"), "return true if this multitude is subset of another multitude, else return false")
        .def("isdisjoint", &Interval::isdisjoint, py::arg("b"), "return true if these multitudes has no common points, else return false")
        .def("add_interval", &Interval::add_interval, py::arg("a"), py::arg("b"), "returns false if all this interval was inside this multitude, else return true")
        .def("add_point", &Interval::add_point, py::arg("a"), "returns false if this point was inside this multitude, else return true. Note that the added point cannot be -INF and +INF")
        .def("remove_interval", &Interval::remove_interval, py::arg("a"), py::arg("b"), "returns false if all this interval was not inside this multitude, else return true")
        .def("remove_point", &Interval::remove_point, py::arg("a"), "returns false if this point was not inside this multitude, else return true. Note that the removed point cannot be -INF and +INF")
        .def("empty", &Interval::empty, "return true if this multitude is empty, else return false")
        .def("points_only", &Interval::points_only, "return true if multitude has only separate points (or empty), else return false")
        .def("clear", &Interval::clear, "clear multitude data")
        .def("inverse", &Interval::inverse, "returns the multitude that is the inverse of the given one")
        .def("__add__", [](const Interval &a, const Interval &b) {return a + b;}, py::is_operator(), py::arg("b"), "returns a new multitude containing the union of the elements of the previous multitudes")
        .def("__or__", [](const Interval &a, const Interval &b) {return a + b;}, py::is_operator(), py::arg("b"), "returns a new multitude containing the union of the elements of the previous multitudes")
        .def("__iadd__", [](Interval &a, const Interval &b) {return a += b;}, py::is_operator(), py::arg("b"), "adds elements of another multitude")
        .def("__ior__", [](Interval &a, const Interval &b) {return a += b;}, py::is_operator(), py::arg("b"), "adds elements of another multitude")
        .def("__sub__", [](const Interval &a, const Interval &b) {return a - b;}, py::is_operator(), py::arg("b"), "returns a new multitude containing the difference of the elements of the previous multitudes")
        .def("__isub__", [](Interval &a, const Interval &b) {return a -= b;}, py::is_operator(), py::arg("b"), "remove elements of another multitude")
        .def("__mul__", [](const Interval &a, const Interval &b) {return a * b;}, py::is_operator(), py::arg("b"), "returns a new multitude containing the intersection of the elements of the previous multitudes")
        .def("__and__", [](const Interval &a, const Interval &b) {return a * b;}, py::is_operator(), py::arg("b"), "returns a new multitude containing the intersection of the elements of the previous multitudes")
        .def("__imul__", [](Interval &a, const Interval &b) {return a *= b;}, py::is_operator(), py::arg("b"), "intersect elements with another multitude")
        .def("__iand__", [](Interval &a, const Interval &b) {return a *= b;}, py::is_operator(), py::arg("b"), "intersect elements with another multitude")
        .def("__xor__", [](const Interval &a, const Interval &b) {return a ^ b;}, py::is_operator(), py::arg("b"), "returns a new multitude containing the symmetric difference of the elements of the previous multitudes")
        .def("__ixor__", [](Interval &a, const Interval &b) {return a ^= b;}, py::is_operator(), py::arg("b"), " generating symmetric difference with elements of another multitude")
        .def("any", [](const Interval &a) {return a.any(true);}, R"doc(
### any

Return any element that is in data.

The function can return `None`, because the returning value does not always exist.

---

- If there is any point, it will be returned.
- If there is an interval `(-INF; +INF)`, the function will return `None`.
- If activated `policy.IntTypePolicy` or `policy.FloatTypePolicy`,
  a smart algorithm will try to find any number in the intervals.
- If it is standard `Interval`, or if the algorithm does not find any element in data,
  the function will return `None`.

---

For custom types and algorithms, consider using this function with additional arguments.
)doc")
        .def("any", static_cast<std::optional<T> (Interval::*)(
                const std::function<std::optional<T>(const T&)> &,
                const std::function<std::optional<T>(const T&)> &,
                const std::function<std::optional<T>(const T&, const T&)> &, T) const>(&Interval::any), py::arg("MINUS_INF_x"), py::arg("x_PLUS_INF"), py::arg("x_y"), py::arg("MINUS_INF_PLUS_INF"), R"doc(
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

A lambdas may return `None`, if the interval has no matching value.
)doc")
        .def("custom_transfer", static_cast<Interval (Interval::*)(const std::function<T(const T&)> &) const>(&Interval::custom_transfer), py::arg("fun"), R"doc(
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
                (const std::function<T(const T&)> &, const typename Interval::inp_type&, const typename Interval::inp_type&) const>(&Interval::custom_transfer), py::arg("fun"), py::arg("MINUS_INF"), py::arg("PLUS_INF"), R"doc(
### custom_transfer()

Transfer all elements that are in this multitude and return a new multitude.

The function takes **one lambda function**
and **two values** – the converted values of `-INF` and `+INF`.

---

- **Lambda** – returns a new value of a point/border of an interval,
  receives one argument - old value of a point/border of an interval.
- **First value** – new value of the border of the interval, that begins from `-INF` - old value of a border of an interval.
- **Second value** – new value of the border of the interval, that ends with `+INF` - old value of a border of an interval.

---

If the first value of an interval becomes greater than the second,
the function will swap them automatically.
)doc")
        .def("apply_policy", static_cast<void (Interval::*)(const interval::policy::empty_print_policy&)>(&Interval::apply_policy), py::arg("policy"), "allow to change any not-type policy")
        .def("apply_policy", static_cast<void (Interval::*)(const interval::policy::minmax_print_policy&)>(&Interval::apply_policy), py::arg("policy"), "allow to change any not-type policy")
    ;
    if constexpr (type_policy::template is_arithmetic_v<T>) cls
        .def("__add__", [](const Interval &a, const T &b) {return a + b;}, py::is_operator(), py::arg("b"), "returns a new multitude with the points shifted forward by the distance val")
        .def("__iadd__", [](Interval &a, const T &b) {return a += b;}, py::is_operator(), py::arg("b"), "shift the points forward by a distance of val")
        .def("__sub__", [](const Interval &a, const T &b) {return a - b;}, py::is_operator(), py::arg("b"), "returns a new multitude with the points shifted backward by the distance val")
        .def("__isub__", [](Interval &a, const T &b) {return a -= b;}, py::is_operator(), py::arg("b"), "shift the points backward by a distance of val")
        .def("__mul__", [](const Interval &a, const T &b) {return a * b;}, py::is_operator(), py::arg("b"), "returns a new multitude with the points multiplied by a factor of val")
        .def("__imul__", [](Interval &a, const T &b) {return a *= b;}, py::is_operator(), py::arg("b"), "multiplies the points of a multitude by a factor of val")
    ;
    if constexpr (type_policy::template is_integral_v<T>) cls
        .def("__floordiv__", [](const Interval &a, const T &b) {return a / b;}, py::is_operator(), py::arg("b"), "returns a new multitude with the points divided by a factor of val")
        .def("__ifloordiv__", [](Interval &a, const T &b) {return a /= b;}, py::is_operator(), py::arg("b"), "divides the points of a multitude by a factor of val")
        .def("__mod__", [](const Interval &a, const T &b) {return a % b;}, py::is_operator(), py::arg("b"), "returns a new multitude with points taken as the remainder of the division by val")
        .def("__imod__", [](Interval &a, const T &b) {return a %= b;}, py::is_operator(), py::arg("b"), "replaces the points with the remainder of the division by val")
    ;
    if constexpr (type_policy::template is_arithmetic_v<T> && !type_policy::template is_integral_v<T>) cls
        .def("__truediv__", [](const Interval &a, const T &b) {return a / b;}, py::is_operator(), py::arg("b"), "returns a new multitude with the points divided by a factor of val")
        .def("__itruediv__", [](Interval &a, const T &b) {return a /= b;}, py::is_operator(), py::arg("b"), "divides the points of a multitude by a factor of val")
    ;
    cls.def_property_readonly_static("minimal", [](const py::object&) { return Min(); }, "return always minimal element in any interval");
    cls.def_property_readonly_static("maximal", [](const py::object&) { return Max(); }, "return always maximal element in any interval");
}

PYBIND11_MODULE(_mathInterval, m) {
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
    const py::module_ policy_mod = m.def_submodule("policy");
    py::class_<interval::policy::int_type_policy> IntTypePolicy(policy_mod, "IntTypePolicy", "internal stored type - int. Additional operations are available");
    py::class_<interval::policy::float_type_policy> FloatTypePolicy(policy_mod, "FloatTypePolicy", "internal stored type - float. Additional operations are available");
    py::class_<interval::policy::unknown_type_policy> UnknownTypePolicy(policy_mod, "UnknownTypePolicy", "may store any types with required operators");

    py::class_<interval::policy::empty_print_policy> EmptyPrintPolicy(policy_mod, "EmptyPrintPolicy", "allows to change how an empty set prints");
    EmptyPrintPolicy.def(py::init<std::string>(), py::arg("s"));
    py::class_<interval::policy::minmax_print_policy> MinMaxPrintPolicy(policy_mod, "MinMaxPrintPolicy", "allows to change how prints -INF and +INF");
    MinMaxPrintPolicy.def(py::init<std::string, std::string>(), py::arg("-INF"), py::arg("+INF"));


    bind_interval<py::object, interval::policy::unknown_type_policy>(m, "_Interval_UnknownTypePolicy");
    bind_interval<FLOAT_TYPE, interval::policy::float_type_policy>(m, "_Interval_FloatTypePolicy");
    bind_interval<INT_TYPE, interval::policy::int_type_policy>(m, "_Interval_IntTypePolicy");
    m.def("Interval", [](const py::object& policy)->py::object {
        if (policy.is(py::type::of<interval::policy::unknown_type_policy>()) || policy.is_none()) return py::cast(new interval::interval<py::object, interval::policy::unknown_type_policy>(), py::return_value_policy::take_ownership);
        if (policy.is(py::type::of<interval::policy::float_type_policy  >())) return py::cast(new interval::interval<FLOAT_TYPE, interval::policy::float_type_policy  >(), py::return_value_policy::take_ownership);
        if (policy.is(py::type::of<interval::policy::int_type_policy    >())) return py::cast(new interval::interval<INT_TYPE, interval::policy::int_type_policy    >(), py::return_value_policy::take_ownership);
        throw py::type_error("Unknown policy");
    }, py::arg("policy") = py::none());

}