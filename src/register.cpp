#include <additional_test_tools.h>
#include <iostream>
#include <register.h>
#include <verifier.h>

int main(const int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    verify::line_checker line(verify::read_file(argv[1]));
    auto a = line.get();
    int now = -1;
    bool changed = false;
    int current_len = 0;
    formatter::vec_reset();
    while (a != std::nullopt) {
        for (auto i = 0; i < formatter::tests.size(); i++) {
            if (verify::same(a.value(),
                             "===== " + formatter::tests[i].name + " =====", true,
                             true, false))
                now = i;
            if (verify::same(a.value(),
                             "===== end of " + formatter::tests[i].name + " =====",
                             true, true, false))
                changed = true, ++current_len;
        }
        if (current_len >= 4) {
            current_len -= 4;
            std::cout << verifier_tests::to_table(formatter::vec, formatter::HEADER)
                << std::endl;
            formatter::vec_reset();
        }
        if (changed)
            now = -1, changed = false;
        else if (now != -1)
            formatter::tests[now].func(a.value());
        a = line.get();
    }
    if (current_len) { std::cout << verifier_tests::to_table(formatter::vec, formatter::HEADER) << std::endl; }
}
