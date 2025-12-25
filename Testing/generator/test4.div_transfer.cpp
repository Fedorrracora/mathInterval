#include <test_base/generate_test_base.h>

TEST(GENERATOR, div_transfer) {
    generate_test::generate_test_base("div_transfer",
                                      [](const interval::interval<int> &a, const int b) {
                                          return a / (b == 0 ? 1 : b);
                                      }, [](const interval::interval<int> &a, const int b) {
                                          // ReSharper disable once CppDeprecatedEntity
                                          return a._div(b == 0 ? 1 : b);
                                      });
}
