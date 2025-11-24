#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>

TEST(ONE_SET, remove_clear_empty_full) {
    interval::interval<int> a;
    verify::line_checker line(verify::read_file("answers/test4.a"));
    EXPECT_TRUE(verify::same(a.print(), line(), true, true, false) && a.empty() && !a.full()) << "error in step 1: testing empty interval\n";
    a.add_interval(1, 111);
    a.add_point(0);
    a.add_point(111);
    a.add_interval(1111, interval::maximal<int>());
    ASSERT_TRUE(verify::same(a.print(), line(), true, true, false) && !a.empty() && !a.full()) << "error in step 2: adding random elements\n";
    a.clear();
    ASSERT_TRUE(verify::same(a.print(), line(), true, true, false) && a.empty() && !a.full()) << "error in step 3: testing clear\n";
    a.add_interval(1, 10);
    a.add_point(1);
    a.add_interval(interval::minimal<int>(), 0);
    a.add_point(15);
    a.add_interval(15, 16);

    a.remove_interval(1, 10);
    a.remove_point(15);
    a.remove_interval(interval::minimal<int>(), 0);
    a.remove_point(1);
    a.remove_interval(15, 16);
    ASSERT_TRUE(verify::same(a.print(), line.last, true, true, false) && a.empty() && !a.full()) << "error in step 4: remove independent intervals and points\n";
    {
        bool b = true;
        b &= a.add_interval(interval::minimal<int>(), interval::maximal<int>());
        b &= a.remove_point(1);
        b &= a.remove_point(0);
        b &= a.remove_point(15);
        b &= a.remove_point(16);
        b &= a.remove_point(-3);
        ASSERT_TRUE(verify::same(a.print(), line(), true, true, false) && !a.empty() && !a.full() && b) << "error in step 5: remove independent points\n";
    }
    {
        bool b = false;
        b |= a.remove_point(0);
        b |= a.remove_point(0);
        b |= a.remove_point(-3);
        ASSERT_TRUE(verify::same(a.print(), line.last, true, true, false) && !a.empty() && !a.full() && !b) << "error in step 6: remove cross points\n";
    }
    {
        bool b = a.remove_interval(-3, 1);
        ASSERT_TRUE(verify::same(a.print(), line(), true, true, false) && !a.empty() && !a.full() && b) << "error in step 7: remove cross interval\n";
    }
    {
        bool b = true;
        b &= a.remove_interval(5, 10);
        b &= !a.remove_point(8);
        ASSERT_TRUE(verify::same(a.print(), line(), true, true, false) && !a.empty() && !a.full() && b) << "error in step 8: remove interval in interval\n";
    }
    {
        bool b = true;
        b &= a.remove_interval(4, 5);
        b &= a.remove_interval(1, 2);
        ASSERT_TRUE(verify::same(a.print(), line(), true, true, false) && !a.empty() && !a.full() && b) << "error in step 9: remove border interval in interval\n";
    }
    {
        bool b = a.remove_interval(3, 12);
        ASSERT_TRUE(verify::same(a.print(), line(), true, true, false) && !a.empty() && !a.full() && b) << "error in step 10: remove interval in mid of intervals\n";
    }
    {
        bool b = a.remove_interval(interval::minimal<int>(), 5);
        ASSERT_TRUE(verify::same(a.print(), line(), true, true, false) && !a.empty() && !a.full() && b) << "error in step 11: remove interval with left infinity border\n";
    }
    {
        bool b = a.remove_interval(10, interval::maximal<int>());
        ASSERT_TRUE(verify::same(a.print(), line(), true, true, false) && a.empty() && !a.full() && b) << "error in step 12: remove interval with right infinity border\n";
    }
    {
        bool b = true;
        b &= a.add_interval(10, interval::maximal<int>());
        b &= a.add_interval(interval::minimal<int>(), 10);
        ASSERT_TRUE(verify::same(a.print(), line(), true, true, false) && !a.empty() && !a.full() && b) << "error in step 13: interval without one point\n";
    }
    {
        bool b = a.add_point(10);
        ASSERT_TRUE(verify::same(a.print(), line(), true, true, false) && !a.empty() && a.full() && b) << "error in step 14: full interval\n";
    }
}