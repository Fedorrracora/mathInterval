#include <test_base/generate_test_base.h>

TEST(GENERATOR, inverse) {
    generate_test::generate_test_base(
        "inverse", [](const interval::interval<int> &a) { return a.inverse(); },
        [](const interval::interval<int> &a) {
            // ReSharper disable once CppDeprecatedEntity
            return a._inverse();
        });
}
