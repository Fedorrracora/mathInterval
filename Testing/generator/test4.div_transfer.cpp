#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>
#include <additional_test_tools.h>
TEST(GENERATOR, div_transfer) {
    auto info = verifier_tests::print_information("GENERATOR TEST (div_transfer)");
    const auto prog = verifier_tests::progress(RANDOM_GENERATOR, "generating && testing", true);
    for (auto iter = 0; iter < RANDOM_GENERATOR; ++iter) {
        prog.call(iter);
        auto a = verifier_tests::generate(25);
        auto b = verify::random_int_distribution(-100, 100);
        if (b == 0) b = 1;
        // ReSharper disable once CppDeprecatedEntity
        EXPECT_TRUE(verify::same(a._div(b).to_string(), (a / b).to_string(), true, true, true)) << "bug in :" << a.to_string();
    }
}