#include <test_base/time_test_base.h>
TEST(TIME_TEST, inverse) {
    time_test::time_test_base("inverse", "inverse()", "_inverse()", [](const interval::interval<int> &a) {
        auto x = a.inverse();
    }, [](const interval::interval<int> &a) {
        // ReSharper disable once CppDeprecatedEntity
        auto x = a._inverse();
    });
}