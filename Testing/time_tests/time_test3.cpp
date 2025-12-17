#include <gtest/gtest.h>
#include <interval.h>
#include <time_test_base.h>
TEST(TIME_TEST, mul_transfer) {
    time_test::time_test_base("mul_transfer", "operator*", "_mul()", [](const interval::interval<int> &a) {
        auto x = a * 2;
    }, [](const interval::interval<int> &a) {
        // ReSharper disable once CppDeprecatedEntity
        auto x = a._mul(2);
    });
}