#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>

TEST(ONE_SET, in_inverse_points_only) {
    interval::interval<int> a;
    verify::line_checker line(verify::read_file("one_set/answers/test6.a"));
    a.add_interval(1, 3);
    ASSERT_TRUE(!a.points_only()) << "error in step 1: points_only on one interval\n";
    a.remove_interval(1, 2);
    a.remove_interval(2, 3);
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && a.points_only()) << "error in step 2: points_only on one point\n";
    a.add_interval(interval::minimal<int>(), 2);
    ASSERT_TRUE(!a.points_only()) << "error in step 3: points_only on interval with -INF\n";
    a.add_interval(2, interval::maximal<int>());
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && !a.points_only() && a.full()) << "error in step 4: points_only on interval with -INF\n";
    a = a.inverse();
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && a.points_only() && !a.full()) << "error in step 5: inverse (-INF; +INF)\n";
    a.add_interval(1, 10);
    a.add_interval(100, interval::maximal<int>());
    a.add_point(10);
    a.add_point(28);
    a.add_point(28);
    EXPECT_TRUE(!a.in(-3) &&
                !a.in(1) &&
                 a.in(3) &&
                 a.in(10) &&
                !a.in(14) &&
                 a.in(28) &&
                !a.in(-7) &&
                !a.in(100) &&
                 a.in(1000) &&
                !a.in(interval::maximal<int>()) &&
                !a.in(interval::minimal<int>())) << "error in step 6: in with point\n";
    a = a.inverse();
    EXPECT_TRUE( verify::same(a.to_string(), line(), CS, WS, BS) &&
                 a.in(3, 3) &&
                 a.in(10, 28) &&
                 a.in(10, 27) &&
                 a.in(11, 28) &&
                 a.in(11, 27) &&
                !a.in(9, 28) &&
                !a.in(10, 29) &&
                !a.in(9, 29) &&
                 a.in(-1000, 1) &&
                 a.in(interval::minimal<int>(), -100) &&
                !a.in(interval::minimal<int>(), interval::maximal<int>()) &&
                !a.in(15, 55) &&
                !a.in(1000, 1001) &&
                 a.in(1000, 1000) &&
                 a.in(1, 1)) << "error in step 7: in with interval after inverse\n";

}