#ifndef PYOBJECT_H
#define PYOBJECT_H
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

namespace py = pybind11;
class PyValue {
public:
    py::object obj{};

    explicit PyValue(py::object o) : obj(std::move(o)) {}
    PyValue() = default;

    bool operator==(const PyValue& other) const {
        py::gil_scoped_acquire gil;

        if (obj.is(other.obj))
            return true;

        return obj.attr("__eq__")(other.obj).cast<bool>();
    }

    std::partial_ordering operator<=>(const PyValue& other) const {
        py::gil_scoped_acquire gil;

        if (obj.is(other.obj))
            return std::partial_ordering::equivalent;

        if (obj.attr("__lt__")(other.obj).cast<bool>())
            return std::partial_ordering::less;

        if (obj.attr("__gt__")(other.obj).cast<bool>())
            return std::partial_ordering::greater;

        if (obj.attr("__eq__")(other.obj).cast<bool>())
            return std::partial_ordering::equivalent;

        return std::partial_ordering::unordered;
    }

    friend std::ostream& operator<<(std::ostream& os, const PyValue& v) {
        py::gil_scoped_acquire gil;
        os << v.obj;
        return os;
    }
};

#endif //PYOBJECT_H