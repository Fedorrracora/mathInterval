#include <test_base/time_test_base.h>
TEST(TIME_TEST, div_transfer) {
    time_test::time_test_base(
        "div_transfer", "operator/", "_div()", [](const interval::interval<int> &a) { return a / 2; },
        [](const interval::interval<int> &a) {
            // ReSharper disable once CppDeprecatedEntity
            return a._div(2);
        });
}
