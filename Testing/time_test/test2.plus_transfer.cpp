#include <test_base/time_test_base.h>
TEST(TIME_TEST, plus_transfer) {
    time_test::time_test_base("plus_transfer", "operator+", "_plus()", [](const interval::interval<int> &a) {
        auto x = a + 10;
    }, [](const interval::interval<int> &a) {
        // ReSharper disable once CppDeprecatedEntity
        auto x = a._plus(10);
    });
}