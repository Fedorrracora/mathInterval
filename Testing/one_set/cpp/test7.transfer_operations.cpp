#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>

TEST(ONE_SET, transfer_operations) {
    interval::interval<int> a;
    verify::line_checker line(verify::read_file("one_set/answers/test7.a"));
    a.add_interval(1, 3);
    a.add_point(8);
    a.add_point(1);
    a.add_point(100);
    a.add_interval(interval::minimal<int>(), -3);
    a.add_interval(100, interval::maximal<int>());
    a += 1;
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 1: +=\n";
    a -= 101;
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 2: -=\n";
    a -= -100;
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 3: -= with negative number\n";
    a += 0;
    ASSERT_TRUE(verify::same(a.to_string(), line.last, CS, WS, BS)) << "error in step 4: += with 0\n";
    a *= 1;
    ASSERT_TRUE(verify::same(a.to_string(), line.last, CS, WS, BS)) << "error in step 5: *= with 1\n";
    a *= 2;
    a.add_interval(-3, 1);
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 6: *=\n";
    a *= -1;
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 7: *= with -1\n";
    a *= -2;
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 8: *= with negative number\n";
    a /= 1;
    ASSERT_TRUE(verify::same(a.to_string(), line.last, CS, WS, BS)) << "error in step 9: /= with 1\n";
    a /= 2;
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 10: /=\n";
    a /= -1;
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 11: /= with -1\n";
    a *= 2;
    a /= -2;
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 12: /= with negative number\n";
    a.add_interval(16, 17);
    a.add_interval(18, 19);
    a /= 2;
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 13: /= with extreme cases\n";
    a.clear();
    a *= 0;
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 14: *= 0 with empty\n";
    a.clear();
    a.add_interval(1, 2);
    a *= 0;
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 15: *= 0 with some data\n";
}
