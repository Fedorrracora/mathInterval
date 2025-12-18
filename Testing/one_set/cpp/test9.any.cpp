#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>
bool correct(const interval::interval<int> &a, const bool empty) {
    auto x = a.any();
    if (empty) {
        return !x.has_value();
    }
    if (!x.has_value()) return false;
    return a.in(x.value());
}

TEST(ONE_SET, any) {
    interval::interval<int> a;
    a.add_interval(1, 4);
    EXPECT_TRUE(correct(a, false)) << "error in step 1: " + a.to_string() +  "\n";
    a.clear();
    a.add_interval(1, 3);
    EXPECT_TRUE(correct(a, false)) << "error in step 2: " + a.to_string() +  "\n";
    a.clear();
    a.add_interval(1, 2);
    EXPECT_TRUE(correct(a, true )) << "error in step 3: " + a.to_string() +  "\n";
    a.clear();
    EXPECT_TRUE(correct(a, true )) << "error in step 4: " + a.to_string() +  "\n";
    a.clear();
    a.add_interval(1, 2);
    a.add_interval(2, 3);
    a.add_interval(3, 4);
    EXPECT_TRUE(correct(a, true )) << "error in step 5: " + a.to_string() +  "\n";
    a.add_point(4);
    EXPECT_TRUE(correct(a, false)) << "error in step 6: " + a.to_string() +  "\n";
    a.clear();
    a.add_interval(-3, -2);
    EXPECT_TRUE(correct(a, true )) << "error in step 7: " + a.to_string() +  "\n";
    a.clear();
    a.add_interval(-3, -1);
    EXPECT_TRUE(correct(a, false)) << "error in step 8: " + a.to_string() +  "\n";
    a.clear();
    a.add_interval(interval::minimal<int>(), -1);
    EXPECT_TRUE(correct(a, false)) << "error in step 9: " + a.to_string() +  "\n";
    a.clear();
    a.add_interval(1, interval::maximal<int>());
    EXPECT_TRUE(correct(a, false)) << "error in step 10: " + a.to_string() +  "\n";
    a.clear();
    a.add_interval(interval::minimal<int>(), interval::maximal<int>());
    EXPECT_TRUE(correct(a, false)) << "error in step 11: " + a.to_string() +  "\n";
}