#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>

TEST(ONE_SET, multi_add) {
    interval::interval<int> a;
    verify::line_checker line(verify::read_file("one_set/answers/test3.a"));
    a.add_interval(1, 10);
    a.add_point(10);
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 1: adding border point\n";
    {
        bool b = a.add_interval(10, 11);
        ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && b) << "error in step 2: meet 2 intervals: last - interval\n";
    }
    a.add_interval(11, 20);
    {
        bool b = a.add_point(11);
        ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && b) << "error in step 3: meet 2 intervals: last - point\n";
    }
    a.add_interval(20, 30);
    a.add_point(30);
    a.add_point(1);
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 4: random additions\n";
    {
        bool b = false;
        b |= a.add_point(1);
        b |= a.add_point(30);
        b |= a.add_point(25);
        b |= a.add_point(10);
        b |= a.add_interval(10, 10);
        b |= a.add_interval(10, 15);
        b |= a.add_interval(10, 20);
        b |= a.add_interval(1, 15);
        b |= a.add_interval(25, 30);
        EXPECT_TRUE(verify::same(a.to_string(), line.last, CS, WS, BS) && !b) << "error in step 5: adding intervals/points that not changing data\n";
    }
    a.add_point(-1);
    a.add_point(0);
    a.add_point(31);
    a.add_point(32);
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 6: points in left and right\n";
    a.add_interval(-10, 1);
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 7: coating interval\n";
    a.add_point(-15);
    a.add_point(-14);
    a.add_point(-13);
    a.add_point(-12);
    a.add_point(-11);
    a.add_point(-10);
    a.add_interval(-20, -15);
    a.add_point(-21);
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 8: points formatting\n";
    a.add_interval(interval::minimal<int>(), -21);
    a.add_interval(35, interval::maximal<int>());
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 9: +-INF\n";
}