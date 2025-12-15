#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>
#include <additional_test_tools.h>
using fp = verifier_tests::fixed_printing;
TEST(TIME_TEST, inverse) {
    auto info = verifier_tests::print_information("TIME TEST (inverse)");
    double coef1, coef2, coef3, coef4;
    for (const int D : {1000, 10}) {
        const auto a = verifier_tests::many_data(D).first;
        double t1, t2;

        {
            auto prog = verifier_tests::progress(ITERATIONS, "testing with inverse()", true, true);
            for (auto iter = 0; iter < ITERATIONS; ++iter) {
                prog.call(iter);
                auto x = a.inverse();
            }
            t1 = prog.stop();
        }

        {
            auto prog = verifier_tests::progress(ITERATIONS, "testing with _inverse()", true, true);
            for (auto iter = 0; iter < ITERATIONS; ++iter) {
                prog.call(iter);
                // ReSharper disable once CppDeprecatedEntity
                auto x = a._inverse();
            }
            t2 = prog.stop();
        }
        const auto t = t2 / t1;
        if (D == 1000) coef1 = t1, coef2 = t2;
        else coef3 = t1, coef4 = t2;
        std::cout << "speed coefficient: " << fp(t) << std::endl;
    }
    std::cout << "inverse() asymptotics coefficient: " << fp(coef1 / coef3 / 100) << std::endl;
    std::cout << "_inverse() asymptotics coefficient: " << fp(coef2 / coef4 / 100) << std::endl;
#ifdef FORMATTER_ENABLED
    std::cout << "----- The following text is needed only for the formatter -----\n";
    std::cout << ITERATIONS << " 1000 10\n";
    std::cout <<  "inverse() " << fp(coef1) << ' ' << fp(coef1 / ITERATIONS) << ' ' <<
        fp(coef3) << ' ' << fp(coef3 / ITERATIONS) << ' ' << fp(coef1 / coef3 / 100) << '\n';
    std::cout << "_inverse() " << fp(coef2) << ' ' << fp(coef2 / ITERATIONS) << ' ' <<
        fp(coef4) << ' ' << fp(coef4 / ITERATIONS) << ' ' << fp(coef2 / coef4 / 100) << '\n' << std::flush;
#endif
}