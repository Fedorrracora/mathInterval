#include <additional_test_tools.h>
#include <verifier.h>
#include <iostream>
#include <utility>

namespace verifier_tests {
    namespace {
        int gen_val() { return verify::random_int_distribution(-MAX_VALUE, MAX_VALUE); }
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

    interval::interval<int> many_data(const int len, const bool verbose) {
        verify::time_checker checker;
        if (verbose) std::cout << "Generating large amounts of data (" << len << " elems)" << std::endl;
        checker.start();
        interval::interval<int> a;
        for (auto i = 0; i < len; ++i) {
            a.add_interval(i * 2, i * 2 + 1);
            a.add_point(i * 2);
        }
        checker.stop();
        if (verbose) std::cout << "Generation done (" << checker.time() << " sec)" << std::endl;
        return a;
    }

    print_information::print_information(std::string s) : test_name(std::move(s)) {
        std::cout << "===== " << test_name << " =====\n" << std::flush;
    }
    print_information::~print_information() {
        std::cout << "===== end of " << test_name << " =====\n" << std::flush;
    }


    void print_information::setup_iterations(const int iter) {
        def_iterations = iter;
    }
    print_information::progress_t print_information::progress(const std::string &s, bool timer) const {
        return {def_iterations, s, timer};
    }


    print_information::progress_t::progress_t(const int iter, const std::string &s, const bool timer) : iterations(iter), debug_iter(iter / 100), t(timer) {
        std::cout << "Run " << iterations << " iterations " << s << std::endl;
        if (t) checker.start();
    }
    void print_information::progress_t::call(const int iter) const {
#ifndef DISABLE_PROGRESS_BAR
        if (iter % debug_iter == 0) {
            std::cout << "\rTesting (" << iter / debug_iter << "/100)" << std::flush;
        }
#endif
    }
    print_information::progress_t::~progress_t() {
        if (t) {
            checker.stop();
            std::cout << "\rTesting done (" << checker.time() << " sec)   \n" << std::flush;
        }
        else {
            std::cout << "\rTesting done          \n" << std::flush;
        }
    }

}