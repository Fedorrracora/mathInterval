#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>

TEST(MANY_SET, equal) {
    interval::interval<int> a, b;
    ASSERT_TRUE(a == b) << "error in step 1: empty\n";
    a.add_interval(1, 2);
    b.add_interval(1, 2);
    ASSERT_TRUE(a == b) << "error in step 2: equal one interval\n";
    a.add_point(2);
    b.add_point(2);
    ASSERT_TRUE(a == b) << "error in step 3: equal one interval and point\n";
    b.remove_point(2);
    ASSERT_TRUE(a != b) << "error in step 4: not equal one point\n";
    a.remove_point(2);
    ASSERT_TRUE(a == b) << "error in step 5: equal one interval\n";
    a.add_interval(2, 3);
    b.clear();
    b.add_interval(1, 3);
    ASSERT_TRUE(a != b) << "error in step 6: not equal without point\n";
    a.add_point(2);
    ASSERT_TRUE(a == b) << "error in step 7: equal\n";
    a.add_interval(100, interval::maximal<int>());
    ASSERT_TRUE(a != b) << "error in step 8: not equal with +INF\n";
    b.add_interval(100, interval::maximal<int>());
    ASSERT_TRUE(a == b) << "error in step 9: equal with +INF\n";
}
