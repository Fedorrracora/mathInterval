#ifndef MATHINTERVAL_GENERATE_TEST_BASE_H
#define MATHINTERVAL_GENERATE_TEST_BASE_H
#include <gtest/gtest.h>
#include <interval.h>

namespace generate_test {
    void generate_test_base(const std::string& test_name,
                            const std::function<interval::interval<int>(const interval::interval<int> &)>& fun1,
                            const std::function<interval::interval<int>(const interval::interval<int> &)>& fun2);
    void generate_test_base(const std::string& test_name,
                            const std::function<interval::interval<int>(const interval::interval<int> &, int)>& fun1,
                            const std::function<interval::interval<int>(const interval::interval<int> &, int)>& fun2);
}
#endif //MATHINTERVAL_GENERATE_TEST_BASE_H