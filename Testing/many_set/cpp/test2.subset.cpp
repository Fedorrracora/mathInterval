#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>

TEST(MANY_SET, subset) {
    interval::interval<int> a, b;
    ASSERT_TRUE(a.issubset(b) && a.issuperset(b) && a.isdisjoint(b)) << "error in step 1: empty\n";
    a.add_point(2);
    ASSERT_TRUE(!a.issubset(b) && a.issuperset(b) && a.isdisjoint(b))
        << "error in step 2: " + a.to_string() + " and " + b.to_string() + "\n";
    b.add_point(2);
    ASSERT_TRUE(a.issubset(b) && a.issuperset(b) && !a.isdisjoint(b))
        << "error in step 3: " + a.to_string() + " and " + b.to_string() + "\n";
    a.add_interval(8, 10);
    EXPECT_TRUE(!a.issubset(b) && a.issuperset(b) && !a.isdisjoint(b))
        << "error in step 4: " + a.to_string() + " and " + b.to_string() + "\n";
    a.clear();
    b.clear();
    a.add_interval(8, 9);
    b.add_interval(9, 10);
    ASSERT_TRUE(!a.issubset(b) && !a.issuperset(b) && a.isdisjoint(b))
        << "error in step 5: " + a.to_string() + " and " + b.to_string() + "\n";
    a = a.inverse();
    ASSERT_TRUE(!a.issubset(b) && a.issuperset(b) && !a.isdisjoint(b))
        << "error in step 6: " + a.to_string() + " and " + b.to_string() + "\n";
    ASSERT_TRUE(a.issubset(a) && a.issuperset(a) && !a.isdisjoint(a)) << "error in step 7: self\n";
    a.clear();
    ASSERT_TRUE(a.issubset(a) && a.issuperset(a) && a.isdisjoint(a)) << "error in step 8: self with empty\n";
}
