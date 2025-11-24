#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <verifier.h>
namespace py = pybind11;
PYBIND11_MODULE(verify_python, m) {
    m.doc() = R"doc(
some testing tools
)doc";
    m.def("read_file", &verify::read_file, py::arg("filename"), "return string with content of file");
    m.def("same", &verify::same, py::arg("a"), py::arg("b"),
                                       py::arg("case_sensitive") = false,
                                       py::arg("whitespace_sensitive") = false,
                                       py::arg("boundary_sensitive") = false, R"doc(
return true if two strings are equal with some filters

---

- case_sensitive - does what it should
- whitespace_sensitive - repetitions of spaces, enter and special characters decodes such one space
- boundary_sensitive - remove all spaces, enters and special characters from begin and end of the string

)doc");
    py::class_<verify::line_checker>(m, "line_checker", "helps to read a long string by lines")
    .def(py::init<std::string>(), py::arg("s"))
    .def("__call__", &verify::line_checker::operator(), "gets next line")
    .def_property_readonly("last", [](const verify::line_checker& self) { return std::string(self.last); }, "last read line");
}