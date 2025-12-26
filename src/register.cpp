#include <additional_test_tools.h>
#include <args.h>
#include <iostream>
#include <register.h>
#include <verifier.h>
bool from_stdin = false;
int table_columns = 4;
ADD_ARG(r, raw, 0, "read from stdin; filename is not using") { from_stdin = true; }
ADD_ARG(l, len, 1, "Sets the maximum number of tests in a single table. One test occupies two columns of the table") {
    table_columns = std::stoi(input.front());
    if (table_columns <= 0) {
        throw std::invalid_argument("Incorrect table size");
    }
}
ADD_ARG(-, help, 0, "print this help") {
    std::cout << "Usage: formatter [args] <filename>" << std::endl;
    print_help();
    exit(0);
}

int main(const int argc, const char *argv[]) {
    const auto lastI = args::init(argc, argv);
    if (argc == 1 || (lastI != argc && from_stdin) || (lastI + 1 != argc && !from_stdin)) {
        std::cout << "Usage: formatter [args] <filename>" << std::endl;
        args::print_help();
        exit(1);
    }
    verify::line_checker line(from_stdin ? "-" : verify::read_file(argv[lastI]));
    auto a = line.get();
    int now = -1;
    bool changed = false;
    int current_len = 0;
    formatter::vec_reset();
    while (a != std::nullopt) {
        for (auto i = 0; i < formatter::tests.size(); i++) {
            if (verify::same(a.value(), "===== " + formatter::tests[i].name + " =====", true, true, false)) now = i;
            if (verify::same(a.value(), "===== end of " + formatter::tests[i].name + " =====", true, true, false))
                changed = true, ++current_len;
        }
        if (current_len >= table_columns) {
            current_len -= table_columns;
            std::cout << verifier_tests::to_table(formatter::vec, formatter::HEADER) << std::endl;
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
