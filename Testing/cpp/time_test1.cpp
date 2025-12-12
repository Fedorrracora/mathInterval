#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>
#include <additional_test_tools.h>

TEST(TIME_TEST, inverse) {
    auto info = verifier_tests::print_information("TIME TEST (inverse)");
    double coef1, coef2;
    for (const int D : {1000, 10}) {
        auto a = verifier_tests::many_data(D);
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
        if (D == 1000) coef1 = t;
        else coef2 = t;
        std::cout << "speed coefficient: " << verifier_tests::fixed_printing(t) << std::endl;
    }
    const auto t = coef1 / coef2;
    std::cout << "asymptotics coefficient: " << verifier_tests::fixed_printing(t) << std::endl;
    EXPECT_TRUE(t > 2) << "too long";
}