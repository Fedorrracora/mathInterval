#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>
#include <additional_test_tools.h>

TEST(GENERATOR, inverse) {
    auto info = verifier_tests::print_information("GENERATOR TEST (inverse)");
    info.setup_iterations(RANDOM_GENERATOR);
    const auto prog = info.progress("", true);
    for (auto iter = 0; iter < RANDOM_GENERATOR; ++iter) {
        prog.call(iter);
        auto a = verifier_tests::generate(25);
        EXPECT_TRUE(verify::same(a.inverse().to_string(), a._inverse().to_string(), true, true, true)) << "bug in :" << a.to_string();
    }
}