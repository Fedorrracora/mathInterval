#include <interval.h>
#include <verifier.h>
#include <additional_test_tools.h>
#include <time_test_base.h>
#include <iostream>
#include <functional>

namespace time_test {
    using fp = verifier_tests::fixed_printing;
    constexpr int HIGH = 1000, LOW = 10;
    constexpr int ATTITUDE = HIGH / LOW;
    void time_test_base(const std::string &name, const std::string &fun1_name, const std::string &fun2_name,
                        const std::function<void(const interval::interval<int>&)> &fun1,
                        const std::function<void(const interval::interval<int>&)> &fun2) {
        auto info = verifier_tests::print_information("TIME TEST (" + name + ")");
        double coef1, coef2, coef3, coef4;
        for (const int D : {HIGH, LOW}) {
            const auto a = verifier_tests::many_data(D).first;
            double t1, t2;

            {
                auto prog = verifier_tests::progress(ITERATIONS, "testing with " + fun1_name, true, true);
                for (auto iter = 0; iter < ITERATIONS; ++iter) {
                    prog.call(iter);
                    fun1(a);
                }
                t1 = prog.stop();
            }

            {
                auto prog = verifier_tests::progress(ITERATIONS, "testing with " + fun2_name, true, true);
                for (auto iter = 0; iter < ITERATIONS; ++iter) {
                    prog.call(iter);
                    fun2(a);
                }
                t2 = prog.stop();
            }
            const auto t = t2 / t1;
            if (D == HIGH) coef1 = t1, coef2 = t2;
            else coef3 = t1, coef4 = t2;
            std::cout << "speed coefficient: " << fp(t) << std::endl;
        }
        std::cout << fun1_name << " asymptotics coefficient: " << fp(coef1 / coef3 / ATTITUDE) << std::endl;
        std::cout << fun2_name << " asymptotics coefficient: " << fp(coef2 / coef4 / ATTITUDE) << std::endl;
#ifdef FORMATTER_ENABLED
        std::cout << "----- The following text is needed only for the formatter -----\n";
        std::cout << ITERATIONS << ' ' << HIGH << ' ' << LOW << '\n';
        std::cout << fun1_name << ' ' << fp(coef1) << ' ' << fp(coef1 / ITERATIONS) << ' ' <<
            fp(coef3) << ' ' << fp(coef3 / ITERATIONS) << ' ' << fp(coef1 / coef3 / ATTITUDE) << '\n';
        std::cout << fun2_name << ' ' << fp(coef2) << ' ' << fp(coef2 / ITERATIONS) << ' ' <<
            fp(coef4) << ' ' << fp(coef4 / ITERATIONS) << ' ' << fp(coef2 / coef4 / ATTITUDE) << '\n' << std::flush;
#endif
    }
}