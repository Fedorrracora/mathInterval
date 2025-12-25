#include <test_base/custom_type_base.h>

namespace type_base {
    test_type::test_type() = default;

    test_type::test_type(const int a) :
        a(a) {}

    bool test_type::operator==(const test_type &other) const { return a == other.a; }
    bool test_type::operator<(const test_type &other) const { return a < other.a; }
    std::ostream &operator<<(std::ostream &os, const test_type &other) { return os << other.a; }
}
