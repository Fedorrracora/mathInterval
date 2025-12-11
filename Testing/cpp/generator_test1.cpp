#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>

constexpr int D = 1000000;
constexpr int debug_iter = RANDOM_GENERATOR / 100;
int gen_val() {
    return verify::random_int_distribution(-D, D);
}
TEST(GENERATOR, inverse) {
    std::cout << "===== GENERATOR TEST (inverse) =====\n" << std::flush;
    verify::time_checker checker;
    checker.start();
    std::cout << "Run " << RANDOM_GENERATOR << " iterations" << std::endl;
    for (auto iter = 0; iter < RANDOM_GENERATOR; ++iter) {
        if (iter % debug_iter == 0) {
            std::cout << "\rTesting (" << iter / debug_iter << "/100)" << std::flush;
        }
        interval::interval<int> a;
        const int n = verify::random_int_distribution(0, 20); // amount add objects
        for (auto i = 0; i < n; ++i) {
            if (verify::rnd() % 2) { // point
                auto val = gen_val();
                a.add_point(val);
            }
            else { // interval
                auto val1 = gen_val(), val2 = gen_val();
                if (val1 > val2) std::swap(val1, val2);
                if (verify::random_int_distribution(0, 10) == 0) {
                    a.add_interval(interval::minimal<int>(), val2);
                }
                else if (verify::random_int_distribution(0, 10) == 0) {
                    a.add_interval(val1, interval::maximal<int>());
                }
                else {
                    a.add_interval(val1, val2);
                }
            }
        }
        EXPECT_TRUE(verify::same(a.inverse().to_string(), a._inverse().to_string(), true, true, true)) << "bug in :" << a.to_string();
    }
    std::cout << "\rTesting done          \n" << std::flush;
    checker.stop();
    std::cout << "Time: " << checker.time() << " sec\n";
    std::cout << "===== end of GENERATOR TEST (inverse) =====\n\n" << std::flush;
}