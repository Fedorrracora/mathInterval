#ifndef MATHINTERVAL_ADDITIONAL_TEST_TOOLS_H
#define MATHINTERVAL_ADDITIONAL_TEST_TOOLS_H
#include <interval.h>
#include <verifier.h>
namespace verifier_tests {
    /// generating random data
    interval::interval<int> generate(int max_len, int probability_coefficient = 15);

    interval::interval<int> many_data(int len, bool verbose = false);

    /// show additional info
    struct print_information {
        const std::string test_name;

        /// s - name of test
        explicit print_information(std::string s);
        ~print_information();

        /// for progress
        void setup_iterations(int iter);

        struct progress_t {
            progress_t(int iter, const std::string &s, bool timer = false);
            ~progress_t();
            /// must be called in each iteration
            void call(int iter) const;
        private:
            int iterations;
            int debug_iter;
            verify::time_checker checker;
            bool t;
        };

        /// print progress bar (s - additional write)
        [[nodiscard]] progress_t progress(const std::string &s, bool timer = false) const;
    private:
        int def_iterations = 0;
    };
}

#endif //MATHINTERVAL_ADDITIONAL_TEST_TOOLS_H