#include <additional_test_tools.h>
#include <args.h>
#include <iostream>
#include <register.h>
#include <verifier.h>
bool from_stdin = true, enable_progress = true;
int table_columns = 4, progress_columns = -1, all_tests = -1, test = 0, failed_tests = 0;
std::string filename;
[[noreturn]] void help(const int exit_code) {
    std::cout << "Usage: ./test_target | ./formatter [args]" << std::endl;
    std::cout << "This tool formats output of test target" << std::endl;
    args::print_args();
    exit(exit_code);
}
ADD_ARG(f, file, 1, "read from file (takes a single argument)") {
    from_stdin = false;
    filename = input.front();
}
ADD_ARG(l, len, 1,
        "Sets the maximum number of tests in a single table. One test occupies two columns of the table (takes a "
        "single argument)") {
    table_columns = std::stoi(input.front());
    if (table_columns <= 0) { throw std::invalid_argument("Incorrect table size"); }
}
ADD_ARG(b, bar, 0, "remove progress bar") { enable_progress = false; }
ADD_ARG(B, bar - colums, 1,
        "add a progress bar and allows you to specify the number of columns (takes a single argument). It can be 0") {
    if (!enable_progress) throw std::invalid_argument("bar already disabled");
    progress_columns = std::stoi(input.front());
    if (progress_columns < 0) throw std::invalid_argument("Incorrect bar size");
}
ADD_ARG(h, help, 0, "print this help") { help(EXIT_SUCCESS); }
void print_progress() {
    if (!enable_progress || all_tests == -1) return;
    std::cout << "test " << test << "/" << all_tests;
    if (progress_columns) {
        std::cout << " [";
        for (auto i = 0; i < progress_columns; ++i) {
            if (i < test * progress_columns / all_tests)
                std::cout << "#";
            else
                std::cout << ".";
        }
        std::cout << "]";
    }
    std::cout << "\r" << std::flush;
}

int main(const int argc, const char *argv[]) {
    if (const auto lastI = args::init(argc, argv); lastI != argc) { help(EXIT_FAILURE); }
    verify::line_checker line(from_stdin ? "-" : verify::read_file(filename));
    auto a = line.get();
    int now = -1;
    bool changed = false, started_test = false;
    int current_len = 0;
    formatter::vec_reset();
    while (a != std::nullopt) {
        if (a.value().starts_with("[==========] Running ")) {
            all_tests = std::stoi(verify::n_word(a.value(), 2));
            if (progress_columns == -1) progress_columns = all_tests;
            if (all_tests <= 0) throw std::invalid_argument("Incorrect number of tests");
        }
        if (a.value().starts_with("[ RUN      ]")) ++test, started_test = true;
        if (a.value().starts_with("[       OK ]")) started_test = false;
        if (a.value().starts_with("[  FAILED  ]")) {
            if (started_test) {
                ++failed_tests;
                std::cout << a.value() << std::endl;
            }
            started_test = false;
        }
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
    if (failed_tests) std::cout << failed_tests << " failed tests";
    for (auto i = 0; i < progress_columns + 9 + std::to_string(test).size() + std::to_string(all_tests).size(); ++i) {
        std::cout << ' ';
    }
    std::cout << std::endl;
}
