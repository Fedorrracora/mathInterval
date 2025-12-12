#ifndef MATHINTERVAL_ADDITIONAL_TEST_TOOLS_H
#define MATHINTERVAL_ADDITIONAL_TEST_TOOLS_H
#include <interval.h>
#include <verifier.h>
namespace verifier_tests {
    /// generating random data
    interval::interval<int> generate(int max_len, int probability_coefficient = 15);

    interval::interval<int> many_data(int len);

    /// simple struct for fixed size printing of float numbers
    struct fixed_printing {
        explicit fixed_printing(const double val) : val(val) {}
        double val;
    };

    std::ostream& operator << (std::ostream& os, const fixed_printing& v);

    /// print progress bar (s - master name)
    struct progress {
        progress(int iter, std::string s, bool timer = false, bool timer_verbose = false);
        ~progress();
        /// must be called in each iteration
        void call(int iter) const;
        /// stop timer and get the time
        double stop();
    private:
        const int iterations;
        const int debug_iter;
        verify::time_checker checker;
        const bool t, t_verbose;
        const std::string master_name;
    };

    /// show additional info
    struct print_information {
        const std::string test_name;

        /// s - name of test
        explicit print_information(std::string s);
        ~print_information();

    private:
        int def_iterations = 0;
    };
}

#endif //MATHINTERVAL_ADDITIONAL_TEST_TOOLS_H