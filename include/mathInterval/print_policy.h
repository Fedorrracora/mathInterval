#ifndef MATHINTERVAL_PRINT_POLICY_H
#define MATHINTERVAL_PRINT_POLICY_H
#include <mathInterval/detail/base.h>
#include <mathInterval/detail/standard_policy.h>
#include <optional>
namespace interval::detail {
    struct print_method {
        [[nodiscard]] virtual constexpr int id() const = 0;
        virtual ~print_method() = default;
    };
}
namespace interval::print_policy {
    struct custom_print_policy final : detail::standard_policy {
        static constexpr std::nullopt_t default_value = std::nullopt;

        custom_print_policy() = default;
        custom_print_policy(const custom_print_policy &other) = default;
        custom_print_policy(custom_print_policy &&other) = default;
        custom_print_policy &operator=(const custom_print_policy &other) = default;
        custom_print_policy &operator=(custom_print_policy &&other) = default;

        custom_print_policy &minmax(std::optional<std::string> min, std::optional<std::string> max) {
            if (!min.has_value()) min = "-INF";
            if (!max.has_value()) max = "+INF";
            min_str = std::move(min);
            max_str = std::move(max);
            return *this;
        }
        custom_print_policy &empty_s(std::string s) {
            empty = std::move(s);
            return *this;
        }
        custom_print_policy &print_method(const detail::print_method &m) {
            method_id = m.id();
            return *this;
        }

    private:
        [[nodiscard]] std::unique_ptr<standard_policy> clone() const override {
            return std::make_unique<custom_print_policy>(*this);
        }
        std::optional<std::string> min_str, max_str, empty;
        int method_id = 0;
    };

    struct standard_print_method final : detail::print_method {
        [[nodiscard]] constexpr int id() const override { return 0; }
    };
    struct no_merge_print_method final : detail::print_method {
        [[nodiscard]] constexpr int id() const override { return 1; }
    };
    struct no_merge_point_to_end_print_method final : detail::print_method {
        [[nodiscard]] constexpr int id() const override { return 2; }
    };
    struct point_to_end_print_method final : detail::print_method {
        [[nodiscard]] constexpr int id() const override { return 3; }
    };

}
#endif // MATHINTERVAL_PRINT_POLICY_H
