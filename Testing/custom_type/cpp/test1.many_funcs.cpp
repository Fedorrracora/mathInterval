#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>
struct test_type {
    int a = 0;
    test_type() = default;
    explicit test_type(const int a) : a(a) {}
    bool operator==(const test_type &other) const { return a == other.a; }
    bool operator<(const test_type &other) const { return a < other.a; }
    friend std::ostream& operator<<(std::ostream& os, const test_type &other) { return os << other.a; }
};
TEST(CUSTOM_TYPE, many_funcs) {
    interval::interval<test_type> a;
    verify::line_checker line(verify::read_file("custom_type/answers/test1.a"));
    a.add_interval(test_type(1), test_type(2));
    a.add_interval(test_type(3), test_type(10));
    a.add_point(test_type(2));
    a.add_point(test_type(3));
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 1: adding intervals and points\n";
    a.remove_point(test_type(5));
    a.remove_interval(test_type(7), test_type(8));
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 2: remove intervals and points\n";
    a = a.inverse();
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 3: inverse\n";
    a.clear();
    a.add_interval(test_type(1), interval::maximal<test_type>());
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 4: +INF\n";
    a.add_interval(interval::minimal<test_type>(), test_type(2));
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && a.full()) << "error in step 5: full\n";
    a.clear();
    a.add_point(test_type(3));
    ASSERT_TRUE(a.any().value() == test_type(3)) << "error in step 6: any\n";
}