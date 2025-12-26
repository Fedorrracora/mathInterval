#ifndef MATHINTERVAL_CUSTOM_TYPE_BASE_H
#define MATHINTERVAL_CUSTOM_TYPE_BASE_H
#include <gtest/gtest.h>
#include <interval.h>
#include <iostream>
#include <verifier.h>

namespace type_base {
    struct test_type {
        int a{};
        test_type();
        explicit test_type(int a);
        bool operator==(const test_type &other) const;
        bool operator<(const test_type &other) const;
        friend std::ostream &operator<<(std::ostream &os, const test_type &other);
    };
} // namespace type_base
#endif // MATHINTERVAL_CUSTOM_TYPE_BASE_H
