#include <gtest/gtest.h>
#include <interval.h>
#include <verifier.h>

TEST(ONE_SET, add_point) {
    interval::interval<int> a;
    verify::line_checker line(verify::read_file(DATA_DIR"one_set/answers/test1.a"));
    {
        bool b = true;
        b &= a.add_point(1);
        b &= a.add_point(2);
        b &= a.add_point(-5);
        b &= a.add_point(0);
        ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && b)
            << "error in step 1: adding independent points\n";
    }
    {
        bool b = false;
        b |= a.add_point(2);
        b |= a.add_point(0);
        EXPECT_TRUE(verify::same(a.to_string(), line.last, CS, WS, BS) && !b)
            << "error in step 2: adding cross points\n";
    }
    EXPECT_ANY_THROW(a.add_point(interval::minimal<int>())) << "error in step 3: throw intervals\n";
    EXPECT_ANY_THROW(a.add_point(interval::maximal<int>())) << "error in step 4: throw intervals\n";
    ASSERT_TRUE(verify::same(a.to_string(), line.last, CS, WS, BS))
        << "error in steps 3-4: throw points: saving data\n";
    {
        bool b = a.add_point(-3);
        EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && b)
            << "error in step 5: adding independent points\n";
    }
}
