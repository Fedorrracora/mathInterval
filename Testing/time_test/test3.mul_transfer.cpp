#include <test_base/time_test_base.h>
TEST(TIME_TEST, mul_transfer) {
    time_test::time_test_base(
        "mul_transfer", "operator*", "_mul()", [](const interval::interval<int> &a) { return a * 2; },
        [](const interval::interval<int> &a) {
            // ReSharper disable once CppDeprecatedEntity
            return a._mul(2);
        });
}
