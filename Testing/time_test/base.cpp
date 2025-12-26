#include <additional_test_tools.h>
#include <functional>
#include <interval.h>
#include <iostream>
#include <test_base/time_test_base.h>
#include <verifier.h>

namespace time_test {
    using fp = verifier_tests::fixed_printing;
    constexpr int HIGH = 1000, LOW = 10;
    constexpr int ATTITUDE = HIGH / LOW;

    void time_test_base(const std::string &name, const std::string &fun1_name, const std::string &fun2_name,
                        const std::function<interval::interval<int>(const interval::interval<int> &)> &fun1,
                        const std::function<interval::interval<int>(const interval::interval<int> &)> &fun2) {
        auto info = verifier_tests::print_information("TIME TEST (" + name + ")");
        double cf1, cf2, cf3, cf4;
        for (const int D : {HIGH, LOW}) {
            const auto a = verifier_tests::many_data(D).first;
            double t1, t2;

            {
                auto prog = verifier_tests::progress(ITERATIONS, "testing with " + fun1_name, true, true);
                for (auto iter = 0; iter < ITERATIONS; ++iter) {
                    prog.call(iter);
                    auto x = fun1(a);
                    verify::DoNotOptimize(x);
                }
                t1 = prog.stop();
            }

            {
                auto prog = verifier_tests::progress(ITERATIONS, "testing with " + fun2_name, true, true);
                for (auto iter = 0; iter < ITERATIONS; ++iter) {
                    prog.call(iter);
                    auto x = fun2(a);
                    verify::DoNotOptimize(x);
                }
                t2 = prog.stop();
            }
            const auto t = t2 / t1;
            if (D == HIGH)
                cf1 = t1, cf2 = t2;
            else
                cf3 = t1, cf4 = t2;
            std::cout << "speed coefficient: " << fp(t) << std::endl;
        }
        std::cout << fun1_name << " asymptotics coefficient: " << fp(cf1 / cf3 / ATTITUDE) << std::endl;
        std::cout << fun2_name << " asymptotics coefficient: " << fp(cf2 / cf4 / ATTITUDE) << std::endl;
#ifdef FORMATTER_ENABLED
        std::cout << "----- The following text is needed only for the formatter -----\n";
        std::cout << ITERATIONS << ' ' << HIGH << ' ' << LOW << '\n';
        std::cout << fun1_name << ' ' << fp(cf1) << ' ' << fp(cf1 / ITERATIONS) << ' ' << fp(cf3) << ' '
                  << fp(cf3 / ITERATIONS) << ' ' << fp(cf1 / cf3 / ATTITUDE) << '\n';
        std::cout << fun2_name << ' ' << fp(cf2) << ' ' << fp(cf2 / ITERATIONS) << ' ' << fp(cf4) << ' '
                  << fp(cf4 / ITERATIONS) << ' ' << fp(cf2 / cf4 / ATTITUDE) << '\n'
                  << std::flush;
#endif
    }
} // namespace time_test
