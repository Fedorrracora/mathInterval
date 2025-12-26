#include <test_base/generate_test_base.h>

TEST(GENERATOR, plus_transfer) {
    generate_test::generate_test_base(
        "plus_transfer", [](const interval::interval<int> &a, const int b) { return a + b; },
        [](const interval::interval<int> &a, const int b) {
            // ReSharper disable once CppDeprecatedEntity
            return a._plus(b);
        });
}
