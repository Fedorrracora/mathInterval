#include <additional_test_tools.h>
#include <args.h>
#include <iostream>
#include <register.h>
#include <verifier.h>
bool from_stdin = false, enable_progress = false;
int table_columns = 4, progress_columns = 20, all_tests = -1, test = 0;
ADD_ARG(r, raw, 0, "read from stdin; filename is not using") { from_stdin = true; }
ADD_ARG(l, len, 1, "Sets the maximum number of tests in a single table. One test occupies two columns of the table (takes a single argument)") {
    table_columns = std::stoi(input.front());
    if (table_columns <= 0) {
        throw std::invalid_argument("Incorrect table size");
    }
}
ADD_ARG(b, bar, 0, "add progress bar") {
    enable_progress = true;
}
ADD_ARG(B, bar-colums, 1, "add a progress bar and allows you to specify the number of columns (takes a single argument)") {
    enable_progress = true;
    progress_columns = std::stoi(input.front());
    if (progress_columns <= 0) {
        throw std::invalid_argument("Incorrect bar size");
    }
}
ADD_ARG(-, help, 0, "print this help") {
    std::cout << "Usage: formatter [args] <filename>" << std::endl;
    print_help();
    exit(0);
}
void print_progress() {
    if (!enable_progress || all_tests == -1) return;
    std::cout << "test " << test << "/" << all_tests << "[";
    for (auto i = 0; i < progress_columns; ++i) {
        if (i < test * progress_columns / all_tests) std::cout << "#";
        else std::cout << ".";
    }
    std::cout << "]\r" << std::flush;
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
        if (a.value().starts_with("[==========] Running ")) {
            all_tests = std::stoi(verify::n_word(a.value(), 2));
            if (all_tests <= 0) throw std::invalid_argument("Incorrect number of tests");
        }
        if (a.value().starts_with("[ RUN      ]")) ++test;
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
        print_progress();
    }
    if (current_len) { std::cout << verifier_tests::to_table(formatter::vec, formatter::HEADER) << std::endl; }
    for (auto i = 0; i < progress_columns + 8 + std::to_string(test).size() + std::to_string(all_tests).size(); ++i) {
        std::cout << ' ';
    }
    std::cout << std::endl;
}
