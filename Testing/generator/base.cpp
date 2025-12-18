#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>
#include <additional_test_tools.h>
#include <functional>
#include <test_base/generate_test_base.h>

namespace generate_test {
    void generate_test_base(const std::string &test_name,
                            const std::function<interval::interval<int>(const interval::interval<int> &, int)> &fun1,
                            const std::function<interval::interval<int>(const interval::interval<int> &, int)> &fun2) {
        auto info = verifier_tests::print_information("GENERATOR TEST (" + test_name + ")");
        const auto prog = verifier_tests::progress(RANDOM_GENERATOR, "generating && testing", true);
        for (auto iter = 0; iter < RANDOM_GENERATOR; ++iter) {
            prog.call(iter);
            auto a = verifier_tests::generate(25);
            const auto b = verify::random_int_distribution(-100, 100);
            EXPECT_TRUE(fun1(a, b).to_string() == fun2(a, b).to_string()) << "bug in :" << a.to_string();
        }
    }
    void generate_test_base(const std::string &test_name,
                            const std::function<interval::interval<int>(const interval::interval<int> &)> &fun1,
                            const std::function<interval::interval<int>(const interval::interval<int> &)> &fun2) {
        generate_test_base(test_name, [&fun1](const interval::interval<int> &a, int){return fun1(a);},
                                     [&fun2](const interval::interval<int> &a, int){return fun2(a);});
    }
}