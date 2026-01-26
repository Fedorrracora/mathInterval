#ifndef MATHINTERVAL_PRINT_POLICY_H
#define MATHINTERVAL_PRINT_POLICY_H
#include <mathInterval/detail/base.h>
namespace interval::print_policy {
    struct custom_print_policy : detail::standard_policy {
        static constexpr std::nullopt_t default_value = std::nullopt;
        std::optional<std::string> min_str, max_str;
        custom_print_policy() = default;
        custom_print_policy &minmax(std::string min, std::string max) {
            min_str = std::move(min);
            max_str = std::move(max);
            return *this;
        }
    };

}
#endif // MATHINTERVAL_PRINT_POLICY_H
