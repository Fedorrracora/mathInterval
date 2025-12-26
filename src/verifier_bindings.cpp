#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <verifier.h>
namespace py = pybind11;

PYBIND11_MODULE(verify_python, m) {
    m.doc() = R"doc(
some testing tools
)doc";
    m.def("read_file", &verify::read_file, py::arg("filename"), "return string with content of file");
    m.def("same", &verify::same, py::arg("a"), py::arg("b"), py::arg("case_sensitive") = false,
          py::arg("whitespace_sensitive") = false, py::arg("boundary_sensitive") = false, R"doc(
return true if two strings are equal with some filters

---

- case_sensitive - does what it should
- whitespace_sensitive - repetitions of spaces, enter and special characters decodes such one space
- boundary_sensitive - remove all spaces, enters and special characters from begin and end of the string

)doc");
    py::class_<verify::line_checker>(m, "line_checker", "helps to read a long string by lines")
        .def(py::init<std::string>(), py::arg("s"))
        .def("__call__", &verify::line_checker::operator(), "gets next line")
        .def("get", &verify::line_checker::get, "gets next line. If you run out of data, None is returned")
        .def_property_readonly(
            "last", [](const verify::line_checker &self) { return std::string(self.last); }, "last read line");
    py::class_<verify::time_checker>(m, "time_checker", "helps keep track of time")
        .def(py::init<>())
        .def("start", &verify::time_checker::start, "start timing")
        .def("stop", &verify::time_checker::stop, "stop timing. Not work if you did not start")
        .def("time", &verify::time_checker::time, "get calculated time");

    m.def("random_int_distribution", &verify::random_int_distribution, py::arg("from"), py::arg("to"),
          "return random value between from and to");
    m.def("find", &verify::find, py::arg("where"), py::arg("what"), py::arg("ch"),
          "finds the index of substring entry into the string");
    m.def("in", &verify::in, py::arg("where"), py::arg("what"), py::arg("ch"),
          "checks for the presence of a substring in the string");
    m.def("change", &verify::change, py::arg("where"), py::arg("from"), py::arg("to"),
          "Change substring `from` to string `to` in string `where`. Return new string, not changes `where`");
    m.def("n_word", &verify::n_word, py::arg("where"), py::arg("n"), "get n`th word in string");
    m.def("boundary", &verify::boundary, py::arg("where"), "removes special symbols around word");
    m.def("boundary_spaces", &verify::boundary_spaces, py::arg("where"), "removes spaces around word");
    m.def("begin_from", &verify::begin_from, py::arg("where"), py::arg("what"), "check if `where` begins with `what`");
    m.def("split", &verify::split, py::arg("s"), py::arg("delim"), "such python split");
    m.def("join", &verify::join, py::arg("sep"), py::arg("data"), "such python join");
}
