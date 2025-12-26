#include <test_base/generate_test_base.h>

TEST(GENERATOR, mul_transfer) {
    generate_test::generate_test_base(
        "mul_transfer", [](const interval::interval<int> &a, const int b) { return a * b; },
        [](const interval::interval<int> &a, const int b) {
            // ReSharper disable once CppDeprecatedEntity
            return a._mul(b);
        });
}
