#include <climits>
#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>

TEST(ONE_SET, add_interval) {
    interval::interval<int> a;
    verify::line_checker line(verify::read_file("one_set/answers/test2.a"));
    {
        bool b = true;
        b &= a.add_interval(1, 5);
        b &= a.add_interval(6, 10);
        b &= a.add_interval(-100, 1);
        ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && b)
            << "error in step 1: adding independent intervals\n";
    }
    {
        bool b = false;
        b |= a.add_interval(11, 11);
        b |= a.add_interval(1, 1);
        b |= a.remove_interval(11, 11);
        b |= a.remove_interval(-1, -1);
        EXPECT_TRUE(verify::same(a.to_string(), line.last, CS, WS, BS) && !b)
            << "error in step 2: adding/removing empty intervals\n";
    }
    EXPECT_ANY_THROW(a.add_interval(0, -1)) << "error in step 3: throw intervals\n";
    EXPECT_ANY_THROW(a.add_interval(0, interval::minimal<int>())) << "error in step 4: throw intervals\n";
    ASSERT_TRUE(verify::same(a.to_string(), line.last, CS, WS, BS))
        << "error in steps 3-4: throw intervals: saving data\n";
    {
        bool b = false;
        b |= a.add_interval(1, 5);
        b |= a.add_interval(1, 4);
        b |= a.add_interval(2, 5);
        EXPECT_TRUE(verify::same(a.to_string(), line.last, CS, WS, BS) && !b)
            << "error in step 5: adding intervals that not changing data\n";
    }
    {
        bool b = a.add_interval(3, 6);
        EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && b)
            << "error in step 6: adding cross intervals\n";
    }
    {
        bool b = a.add_interval(0, 8);
        ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && b)
            << "error in step 7: adding cross intervals\n";
    }
    {
        bool b = true;
        b &= a.add_interval(12, 13);
        b &= a.add_interval(11, 12);
        EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && b)
            << "error in step 8: adding independent intervals\n";
    }
    {
        bool b = a.add_interval(11, 13);
        ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && b)
            << "error in step 9: adding cross intervals\n";
    }
    {
        bool b = a.add_interval(15, interval::maximal<int>());
        ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && b)
            << "error in step 10: adding cross infinity intervals\n";
    }
    {
        bool b = a.add_interval(interval::minimal<int>(), 12);
        ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && b)
            << "error in step 11: adding cross infinity intervals\n";
    }
    {
        bool b = a.add_interval(INT_MIN, INT_MAX);
        ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && b)
            << "error in step 12: adding cross intervals\n";
    }
    {
        bool b = a.add_interval(interval::minimal<int>(), interval::maximal<int>());
        ASSERT_TRUE(verify::same(a.to_string(), line.last, CS, WS, BS) && !b)
            << "error in step 13: adding independent infinity intervals\n";
    }
}
