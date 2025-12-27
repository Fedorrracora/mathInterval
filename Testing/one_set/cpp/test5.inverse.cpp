#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>

TEST(ONE_SET, inverse) {
    interval::interval<int> a;
    verify::line_checker line(verify::read_file(DATA_DIR"one_set/answers/test5.a"));

    a.add_interval(1, 2);
    EXPECT_TRUE(verify::same(a.inverse().to_string(), line(), CS, WS, BS)) << "error in step 1: inverse one interval\n";
    a.clear();

    a.add_point(5);
    EXPECT_TRUE(verify::same(a.inverse().to_string(), line(), CS, WS, BS)) << "error in step 2: inverse one point\n";
    a.clear();

    a.add_interval(1, 10);
    a.add_point(1);
    a.add_point(10);
    EXPECT_TRUE(verify::same(a.inverse().to_string(), line(), CS, WS, BS))
        << "error in step 3: inverse one interval with points around\n";
    a.clear();

    EXPECT_TRUE(verify::same(a.inverse().to_string(), line(), CS, WS, BS))
        << "error in step 4: inverse empty interval\n";

    a.add_interval(interval::minimal<int>(), interval::maximal<int>());
    EXPECT_TRUE(verify::same(a.inverse().to_string(), line(), CS, WS, BS))
        << "error in step 5: inverse interval (-INF; +INF)\n";
    a.clear();

    a.add_point(-3);
    a.add_point(0);
    a.add_point(1);
    a.add_interval(1, 2);
    a.add_point(2);
    a.add_interval(5, 10);
    a.add_point(10);
    a.add_point(11);
    a.add_interval(11, 100);
    a.add_interval(105, 110);
    a.add_point(1000);
    a.add_interval(1001, 1002);
    a.add_interval(1002, 1003);
    a.add_interval(1003, 1004);
    a.add_point(1005);
    a.add_interval(1006, 1007);
    a.add_point(1007);
    a.add_point(1008);
    a.add_interval(1009, 1010);
    a.add_interval(1012, 1013);
    a.add_interval(1015, 1016);
    EXPECT_TRUE(verify::same(a.inverse().to_string(), line(), CS, WS, BS))
        << "error in step 5: inverse many random points and intervals\n";
    a.clear();

    a.add_interval(interval::minimal<int>(), 10);
    EXPECT_TRUE(verify::same(a.inverse().to_string(), line(), CS, WS, BS))
        << "error in step 5: inverse many random points and intervals\n";
    a.clear();

    a.add_interval(10, interval::maximal<int>());
    EXPECT_TRUE(verify::same(a.inverse().to_string(), line(), CS, WS, BS))
        << "error in step 5: inverse many random points and intervals\n";
    a.clear();
}
