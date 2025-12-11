#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>

constexpr int debug_iter = ITERATIONS / 100;
TEST(TIME_TEST, inverse) {
    verify::time_checker checker;
    interval::interval<int> a;
    auto print_time = [](const double t) {
        std::cout << "All time: " << t << " sec\n";
        std::cout << "Mean time for run: "
                  << (t / ITERATIONS) << " sec\n\n";
        std::cout << std::flush;
    };
    std::cout << "Generating large amounts of data" << std::endl;
    for (auto i = 0; i < 1000; i += 2) {
        a.add_interval(i, i + 1);
        a.add_point(i);
    }

    checker.start();
    std::cout << "Generation done. Run " << ITERATIONS << " iterations with a.inverse()." << std::endl;
    for (auto iter = 0; iter < ITERATIONS; ++iter) {
        if (iter % debug_iter == 0) {
            std::cout << "\rTesting (" << iter / debug_iter << "/100)" << std::flush;
        }
        auto x = a.inverse();
    }
    std::cout << "\rTesting done          \n" << std::flush;
    checker.stop();
    double t1 = checker.time();
    print_time(t1);

    checker.start();
    std::cout << "Run " << ITERATIONS << " iterations with a._inverse()." << std::endl;
    for (auto iter = 0; iter < ITERATIONS; ++iter) {
        if (iter % debug_iter == 0) {
            std::cout << "\rTesting (" << iter / debug_iter << "/100)" << std::flush;
        }
        auto x = a._inverse();
    }
    std::cout << "\rTesting done          \n" << std::flush;
    checker.stop();
    double t2 = checker.time();
    print_time(t2);
    ASSERT_TRUE(t2 / t1 > 5);
}