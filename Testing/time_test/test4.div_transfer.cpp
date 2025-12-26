#include <test_base/time_test_base.h>
TEST(TIME_TEST, div_transfer) {
    time_test::time_test_base(
        "div_transfer", "operator/", "_div()", [](const interval::interval<int> &a) { auto x = a / 2; },
        [](const interval::interval<int> &a) {
            // ReSharper disable once CppDeprecatedEntity
            auto x = a._div(2);
        });
}
