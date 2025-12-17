#include <iostream>
#include <verifier.h>
#include <register.h>
#include <additional_test_tools.h>

int main(const int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    verify::line_checker line(verify::read_file(argv[1]));
    auto a = line.get();
    int now = -1;
    bool changed = false;
    while (a != std::nullopt) {
        for (auto i = 0; i < formatter::tests.size(); i++) {
            if (verify::same(a.value(), "===== " + formatter::tests[i].name + " =====", true, true, false)) now = i;
            if (verify::same(a.value(), "===== end of " + formatter::tests[i].name + " =====", true, true, false)) changed = true;
        }
        if (changed) {
        }
        else if (now != -1) formatter::tests[now].func(a.value());
        a = line.get();
        if (changed) now = -1, changed = false;
    }
    std::cout << verifier_tests::to_table(formatter::vec, formatter::HEADER);
}
