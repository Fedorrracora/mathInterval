#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>
#include <additional_test_tools.h>

TEST(TIME_TEST, inverse) {
    auto info = verifier_tests::print_information("TIME TEST (inverse)");
    info.setup_iterations(ITERATIONS);
    verify::time_checker checker;
    auto print_time = [](const double t) {
        std::cout << "All time: " << t << " sec\n";
        std::cout << "Mean time for run: "
                  << (t / ITERATIONS) << " sec\n\n";
        std::cout << std::flush;
    };

    auto a = verifier_tests::many_data(1000, true);

    checker.start();
    {
        const auto prog = info.progress("with a.inverse()");
        for (auto iter = 0; iter < ITERATIONS; ++iter) {
            prog.call(iter);
            auto x = a.inverse();
        }
    }
    checker.stop();
    const double t1 = checker.time();
    print_time(t1);

    checker.start();
    {
        const auto prog = info.progress("with a._inverse()");
        for (auto iter = 0; iter < ITERATIONS; ++iter) {
            prog.call(iter);
            // ReSharper disable once CppDeprecatedEntity
            auto x = a._inverse();
        }
    }
    checker.stop();
    const double t2 = checker.time();
    print_time(t2);
    const auto t = t2 / t1;
    std::cout << "speed coefficient: " << t << std::endl;
    ASSERT_TRUE(t > 2) << "too long";
}