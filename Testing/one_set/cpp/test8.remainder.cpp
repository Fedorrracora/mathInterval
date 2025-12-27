#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>

TEST(ONE_SET, remainder) {
    interval::interval<int> a;
    verify::line_checker line(verify::read_file(DATA_DIR"one_set/answers/test8.a"));
    a.add_interval(1, 3);
    a %= 4;
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS))
        << "error in step 1: %= with one interval not changing data\n";
    a %= 3;
    ASSERT_TRUE(verify::same(a.to_string(), line.last, CS, WS, BS))
        << "error in step 2: %= with one interval not changing data\n";
    a %= 2;
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 3: %= with one interval\n";
    a.clear();
    a.add_interval(1, 4);
    a %= 2;
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS))
        << "error in step 4: %= with one interval to full interval\n";
    a.clear();
    a %= 3;
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 5: %= with empty interval\n";
    a.clear();
    a.add_interval(-1, 0);
    a %= 3;
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 6: %= with negative interval\n";
    a.clear();
    a.add_interval(-1, 1);
    a %= 3;
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS))
        << "error in step 7: %= with interval with positive and negative borders\n";
    a.clear();
    a.add_interval(0, 3);
    a %= 3;
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS))
        << "error in step 8: %= with interval with equal values on the balance\n";
    a.clear();
    a.add_interval(0, 6);
    a %= 3;
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS))
        << "error in step 9: %= with interval with equal values on the balance\n";
    a.clear();
    a.add_point(1);
    a.add_interval(1, 3);
    a.add_point(3);
    a %= 3;
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 10: %= with interval and points\n";
    a.clear();
    a.add_point(-1);
    a %= 3;
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 11: %= with negative point\n";
    a.clear();
    a.add_interval(0, interval::maximal<int>());
    a %= 2;
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 12: %= with infinity border\n";
}
