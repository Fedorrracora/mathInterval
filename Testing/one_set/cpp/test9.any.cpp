#include <gtest/gtest.h>
#include <interval.h>
#include <string>
#include <verifier.h>

template <typename T>
bool correct(const interval::interval<T> &a, const bool empty) {
    auto x = a.any();
    if (empty) { return !x.has_value(); }
    if (!x.has_value()) return false;
    return a.in(x.value());
}

TEST(ONE_SET, any) {
    interval::interval<int> a;
    interval::interval<std::string> b;
    a.add_interval(1, 4);
    EXPECT_TRUE(correct(a, false)) << "error in step 1: " + a.to_string() + "\n";
    a.clear();
    a.add_interval(1, 3);
    EXPECT_TRUE(correct(a, false)) << "error in step 2: " + a.to_string() + "\n";
    a.clear();
    a.add_interval(1, 2);
    EXPECT_TRUE(correct(a, true)) << "error in step 3: " + a.to_string() + "\n";
    a.clear();
    EXPECT_TRUE(correct(a, true)) << "error in step 4: " + a.to_string() + "\n";
    a.clear();
    a.add_interval(1, 2);
    a.add_interval(2, 3);
    a.add_interval(3, 4);
    EXPECT_TRUE(correct(a, true)) << "error in step 5: " + a.to_string() + "\n";
    a.add_point(4);
    EXPECT_TRUE(correct(a, false)) << "error in step 6: " + a.to_string() + "\n";
    a.clear();
    a.add_interval(-3, -2);
    EXPECT_TRUE(correct(a, true)) << "error in step 7: " + a.to_string() + "\n";
    a.clear();
    a.add_interval(-3, -1);
    EXPECT_TRUE(correct(a, false)) << "error in step 8: " + a.to_string() + "\n";
    a.clear();
    a.add_interval(interval::minimal<int>(), -1);
    EXPECT_TRUE(correct(a, false)) << "error in step 9: " + a.to_string() + "\n";
    a.clear();
    a.add_interval(1, interval::maximal<int>());
    EXPECT_TRUE(correct(a, false)) << "error in step 10: " + a.to_string() + "\n";
    a.clear();
    a.add_interval(interval::minimal<int>(), interval::maximal<int>());
    EXPECT_TRUE(correct(a, false)) << "error in step 11: " + a.to_string() + "\n";

    b.add_interval("a", "b");
    EXPECT_TRUE(correct(b, false)) << "error in step 12: " + b.to_string() + "\n";
    b.clear();
    b.add_interval("a", "aaa");
    EXPECT_TRUE(correct(b, false)) << "error in step 13: " + b.to_string() + "\n";
    b.clear();
    b.add_interval("a", "aa");
    b.add_interval("aa", "aaa");
    EXPECT_TRUE(correct(b, true)) << "error in step 14: " + b.to_string() + "\n";
    b.clear();
    b.add_interval("a", "z");
    EXPECT_TRUE(correct(b, false)) << "error in step 15: " + b.to_string() + "\n";
    b.clear();
    EXPECT_TRUE(correct(b, true)) << "error in step 16: " + b.to_string() + "\n";
    b.add_interval("a", interval::maximal<std::string>());
    EXPECT_TRUE(correct(b, false)) << "error in step 17: " + b.to_string() + "\n";
    b.clear();
    b.add_interval(interval::minimal<std::string>(), interval::maximal<std::string>());
    EXPECT_TRUE(correct(b, false)) << "error in step 18: " + b.to_string() + "\n";
}
