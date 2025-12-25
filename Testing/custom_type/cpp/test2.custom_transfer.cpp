#include <test_base/custom_type_base.h>
using test_type = type_base::test_type;
TEST(CUSTOM_TYPE, custom_transfer) {
    interval::interval<test_type> a;
    verify::line_checker line(verify::read_file("custom_type/answers/test2.a"));
    a.add_interval(test_type(1), test_type(2));
    a.add_interval(test_type(4), test_type(10));
    a.add_point(test_type(4));
    a = a.custom_transfer([](const test_type &t) { return test_type(t.a + 1); });
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 1: += 1\n";
    a.add_interval(test_type(11), interval::maximal<test_type>());
    a = a.custom_transfer([](const test_type &t) { return test_type(-t.a); }, interval::maximal<test_type>(),
                          interval::minimal<test_type>());
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 2: *= -1\n";
    a = a.custom_transfer([](const test_type &t) { return t; }, test_type(-100), test_type(100));
    ASSERT_TRUE(verify::same(a.to_string(), line(), CS, WS, BS)) << "error in step 3: set borders\n";
}
