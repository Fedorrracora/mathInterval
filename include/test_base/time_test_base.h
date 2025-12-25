#ifndef MATHINTERVAL_TIME_TEST_BASE_H
#define MATHINTERVAL_TIME_TEST_BASE_H
#include <gtest/gtest.h>
#include <interval.h>

namespace time_test {
    void time_test_base(const std::string& name, const std::string& fun1_name, const std::string& fun2_name,
                        const std::function<void(const interval::interval<int> &)>& fun1,
                        const std::function<void(const interval::interval<int> &)>& fun2);
}

#endif //MATHINTERVAL_TIME_TEST_BASE_H