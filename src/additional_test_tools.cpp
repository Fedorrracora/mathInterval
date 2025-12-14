#include <additional_test_tools.h>
#include <verifier.h>
#include <iostream>
#include <utility>
#ifdef DISABLE_PROGRESS_BAR
#define CONTROL_CHAR ""
#else
#define CONTROL_CHAR "\r"
#endif
constexpr int PRECISION = 10;

namespace verifier_tests {
    namespace {
        int gen_val() { return verify::random_int_distribution(-MAX_VALUE, MAX_VALUE); }
    }

    std::ostream& operator << (std::ostream& os, const fixed_printing& v) {
        return os << std::fixed << std::setprecision(PRECISION) << v.val;
    }

    interval::interval<int> generate(const int max_len, const int probability_coefficient) {
        interval::interval<int> a;
        const int n = verify::random_int_distribution(0, max_len); // amount add objects
        for (auto i = 0; i < n; ++i) {
            if (verify::rnd() % 2) { // point
                auto val = gen_val();
                a.add_point(val);
            }
            else { // interval
                auto val1 = gen_val(), val2 = gen_val();
                if (val1 > val2) std::swap(val1, val2);
                if (verify::random_int_distribution(0, probability_coefficient) == 0) {
                    a.add_interval(interval::minimal<int>(), val2);
                }
                else if (verify::random_int_distribution(0, probability_coefficient) == 0) {
                    a.add_interval(val1, interval::maximal<int>());
                }
                else {
                    a.add_interval(val1, val2);
                }
            }
        }
        return a;
    }

    std::pair<interval::interval<int>, double> many_data(const int len) {
        progress prog(len, "Generating " + std::to_string(len) + " elems of data", true);
        interval::interval<int> a;
        for (auto i = 0; i < len; ++i) {
            a.add_interval(i * 2, i * 2 + 1);
            a.add_point(i * 2);
        }
        return {a, prog.stop()};
    }

    print_information::print_information(std::string s) : test_name(std::move(s)) {
        std::cout << "===== " << test_name << " =====\n" << std::flush;
    }
    print_information::~print_information() {
        std::cout << "===== end of " << test_name << " =====\n" << std::flush;
    }



    progress::progress(const int iter, std::string s, const bool timer, const bool timer_verbose) : iterations(iter), debug_iter(iter / 100), t(timer), t_verbose(timer_verbose), master_name(std::move(s)) {
        if (t) checker.start();
    }
    void progress::call(const int iter) const {
#ifndef DISABLE_PROGRESS_BAR
        if (iter % debug_iter == 0) {
            std::cout << '\r' << master_name << ": " << iter / debug_iter << "/100" << std::flush;
        }
#endif
    }
    double progress::stop() {
        checker.stop();
        return checker.time();
    }

    progress::~progress() {
        if (t) {
            checker.stop();
            if (t_verbose)
                std::cout << CONTROL_CHAR << master_name << ": done (" << fixed_printing(checker.time()) << " sec, "
                        << fixed_printing(checker.time() / iterations) << " sec for iteration)\n" << std::flush;
            else std::cout << CONTROL_CHAR << master_name << ": done (" << fixed_printing(checker.time()) << " sec)   \n" << std::flush;
        }
        else {
            std::cout << CONTROL_CHAR << master_name << ": done          \n" << std::flush;
        }
    }

    std::string to_table(std::vector<std::vector<std::string>> &vec, const int HEADER_LINE) {
        vec[HEADER_LINE].resize(vec.front().size());
        for (auto depth = 0; depth < vec.front().size(); ++depth) {
            std::size_t max = 0;
            for (auto &i : vec) {
                verify::maxof(max, i[depth].size());
            }
            for (auto &i : vec) {
                while (i[depth].size() < max) i[depth] += ' ';
            }
            for (auto &i : vec[HEADER_LINE][depth]) {
                i = '-';
            }
        }
        std::string out;
        for (auto i = 0; i < vec.size(); ++i) {
            const char control_char = i == HEADER_LINE ? '-':' ';
            out += '|';
            for (const auto &j : vec[i]) {
                out += control_char;
                out += j;
                out += control_char;
                out += '|';
            }
            out += '\n';
        }
        return out;
    }
}