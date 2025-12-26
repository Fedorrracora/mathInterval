#include <iostream>
#include <verifier.h>
/// python tests from 5 are generated from cpp tests using this tool
std::string tab = "    ";

int main(const int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    verify::line_checker line(verify::read_file(argv[1]));
    std::cout << "import _mathInterval as interval\n"
                 "import verify_python as verify\n"
                 "\n"
                 "stp = (True, True, True)\n";
    bool in_assert = false;
    for (auto a = line.get(); a != std::nullopt; a = line.get()) {
        if (a.value()[0] == '#') continue;
        a.value() = verify::boundary_spaces(a.value());
        if (a == "}") continue;
        if (a == "{") a.value() = "";

        a.value() = verify::change(a.value(), ";", "");
        a.value() = verify::change(a.value(), "interval::minimal<int>()", "a.minimal");
        a.value() = verify::change(a.value(), "interval::minimal<test_type>()", "a.minimal");
        a.value() = verify::change(a.value(), "interval::maximal<int>()", "a.maximal");
        a.value() = verify::change(a.value(), "interval::maximal<test_type>()", "a.maximal");
        a.value() = verify::change(a.value(), "false", "False");
        a.value() = verify::change(a.value(), "true", "True");
        a.value() = verify::change(a.value(), "to_string()", "__str__()");

        const auto data = verify::split(a.value());
        if (!data.empty() && data.front() == "interval::interval<int>") {
            std::vector<std::string> names, mul;
            for (auto i = 1; i < data.size(); ++i)
                names.push_back(verify::boundary(data[i])), mul.emplace_back("interval.Interval()");
            a.value() = verify::join(", ", names) + " = " + verify::join(", ", mul);
        }
        if (!data.empty() && data.front() == "verify::line_checker") {
            a.value() = "line = " + verify::change(verify::change(a.value(), "::", "."), " line", "");
        }
        if (in_assert || verify::begin_from(a.value(), "EXPECT_TRUE") || verify::begin_from(a.value(), "ASSERT_TRUE")) {
            in_assert = true;
            a.value() = verify::change(a.value(), "EXPECT_TRUE", "assert");
            a.value() = verify::change(a.value(), "ASSERT_TRUE", "assert");
            a.value() = verify::change(a.value(), " << ", ", ");
            a.value() = verify::change(a.value(), "!", "1 - ");
            a.value() = verify::change(a.value(), "1 - =", "!=");
            a.value() = verify::change(a.value(), "&&", "and");
            a.value() = verify::change(a.value(), "||", "or");
            a.value() = verify::change(a.value(), "CS, WS, BS", "*stp");
            a.value() = verify::change(a.value(), "\\n", "");
            a.value() = verify::change(a.value(), "::", ".");
            a.value() = verify::change(a.value(), ".in(", ".contains(");
        }
        if (!a.value().empty() && a.value().back() == '"') in_assert = false;

        a.value() = tab + a.value();
        if (a.value().substr(tab.size(), 4) == "TEST") {
            a.value() = "def test_" + verify::boundary(verify::n_word(a.value(), 1)) + "():";
        }
        std::cout << a.value() << std::endl;
    }
}
