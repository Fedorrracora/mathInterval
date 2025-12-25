#include <test_base/custom_type_base.h>
using test_type = type_base::test_type;
TEST(CUSTOM_TYPE, many_funcs) {
    interval::interval<test_type> a;
    verify::line_checker line(verify::read_file("custom_type/answers/test1.a"));
    a.add_interval(test_type(1), test_type(2));
    a.add_interval(test_type(3), test_type(10));
    a.add_point(test_type(2));
    a.add_point(test_type(3));
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 1: adding intervals and points\n";
    a.remove_point(test_type(5));
    a.remove_interval(test_type(7), test_type(8));
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 2: remove intervals and points\n";
    a = a.inverse();
    EXPECT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 3: inverse\n";
    a.clear();
    a.add_interval(test_type(1), interval::maximal<test_type>());
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 4: +INF\n";
    a.add_interval(interval::minimal<test_type>(), test_type(2));
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS) && a.full()) << "error in step 5: full\n";
    a.clear();
    a.add_point(test_type(3));
    ASSERT_TRUE(a.any().value() == test_type(3)) << "error in step 6: any\n";
}
