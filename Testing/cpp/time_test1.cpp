#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>
#include <additional_test_tools.h>

TEST(TIME_TEST, inverse) {
    auto info = verifier_tests::print_information("TIME TEST (inverse)");
    auto a = verifier_tests::many_data(1000);
    double t1, t2;

    {
        auto prog = verifier_tests::progress(ITERATIONS, "testing with a.inverse()", true, true);
        for (auto iter = 0; iter < ITERATIONS; ++iter) {
            prog.call(iter);
            auto x = a.inverse();
        }
        t1 = prog.stop();
    }

    {
        auto prog = verifier_tests::progress(ITERATIONS, "testing with a._inverse()", true, true);
        for (auto iter = 0; iter < ITERATIONS; ++iter) {
            prog.call(iter);
            // ReSharper disable once CppDeprecatedEntity
            auto x = a._inverse();
        }
        t2 = prog.stop();
    }
    const auto t = t2 / t1;
    std::cout << "speed coefficient: " << t << std::endl;
    ASSERT_TRUE(t > 2) << "too long";
}