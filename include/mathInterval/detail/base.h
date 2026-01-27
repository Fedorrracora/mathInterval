#ifndef MATHINTERVAL_BASE_H
#define MATHINTERVAL_BASE_H
#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
namespace interval::detail {
    /// allow to custom detecting type
    struct type_policy {};
    /// standard type for print methods
    struct print_method;
    /// for concept
    struct custom_type_policy_flag : type_policy {};

    /// default interval::interval concept for type_policy
    template <typename P>
    concept type_policy_c = std::is_base_of_v<type_policy, P>;

    /// std::set comparator of inner_type
    template <typename T>
    struct pair_less {
        using inner_type = std::pair<int, T>;
        // activate transparent policy
        using is_transparent = void;

        [[nodiscard]] bool operator()(const inner_type &a, const inner_type &b) const noexcept;
        [[nodiscard]] bool operator()(const std::pair<inner_type, inner_type> &a,
                                      const std::pair<inner_type, inner_type> &b) const noexcept;
        [[nodiscard]] bool operator()(const T &a, const T &b) const noexcept;

        /// comparing first element of `a` only. For lower_bound
        [[nodiscard]] bool operator()(const std::pair<inner_type, inner_type> &a, const inner_type &b) const noexcept;
        /// comparing first element of `b` only. For lower_bound
        [[nodiscard]] bool operator()(const inner_type &a, const std::pair<inner_type, inner_type> &b) const noexcept;

        /// a like point `(1; a)`
        [[nodiscard]] bool operator()(const T &a, const inner_type &b) const noexcept;
        /// b like point `(1; b)`
        [[nodiscard]] bool operator()(const inner_type &a, const T &b) const noexcept;

        /// a like point `(1; a)`. comparing first element of `b` only
        [[nodiscard]] bool operator()(const T &a, const std::pair<inner_type, inner_type> &b) const noexcept;
        /// b like point `(1; b)`. comparing first element of `a` only
        [[nodiscard]] bool operator()(const std::pair<inner_type, inner_type> &a, const T &b) const noexcept;
    };

    /// concept for checking custom_type_policy
    template <typename P>
    concept custom_type_policy_c =
        std::is_base_of_v<custom_type_policy_flag, P> && !std::is_same_v<custom_type_policy_flag, P>;
    template <typename P>
    concept not_custom_type_policy_c = !custom_type_policy_c<P>;

    /// standard policy handler
    template <typename T, type_policy_c type_policy>
    class temp_policy_wrapper;

    /// standard type for policies
    struct standard_policy {
        virtual ~standard_policy() = default;

    private:
        [[nodiscard]] virtual std::unique_ptr<standard_policy> clone() const = 0;
        virtual bool push(const std::unique_ptr<standard_policy> &el) = 0;
        template <typename T, type_policy_c type_policy>
        friend class temp_policy_wrapper;
    };
} // namespace interval::detail

namespace interval::detail {
    struct default_config {
        constexpr static auto min_str = "-INF", max_str = "+INF", empty = "*Empty*", sep = "; ", unite = " U ";
    };
}

namespace interval::detail::custom_type {
    /// returns T{} unless overridden by type policies. The return value is used as a placeholder. Its value is never
    /// used
    template <typename T, not_custom_type_policy_c>
    T get_value() {
        return T{};
    }
    /// returns T{} unless overridden by type policies. The return value is used as a placeholder. Its value is never
    /// used
    template <typename T, custom_type_policy_c type_policy>
    T get_value();

    /// use std::ostream operator << for interval::interval::to_string unless overridden by type policies.
    template <typename T, not_custom_type_policy_c>
    [[nodiscard]] std::string to_str(T el);

    /// use std::ostream operator << for interval::interval::to_string unless overridden by type policies.
    template <typename T, custom_type_policy_c type_policy>
    [[nodiscard]] std::string to_str(T el);

} // namespace interval::detail::custom_type

namespace interval::type_policy {
    /// standard type detection
    struct standard_type_policy;
    /// type detects as int
    struct int_type_policy;
    /// type detects as float
    struct float_type_policy;
    /// type detects as string
    struct string_type_policy;
    /// for custom control. These features are described in <mathInterval/type_policy.h>
    template <typename T>
    struct custom_type_policy;
} // namespace interval::type_policy

namespace interval::print_policy {
    /// for custom control. These features are described in <mathInterval/print_policy.h>
    struct custom_print_policy;

    struct standard_print_method;
    struct no_merge_print_method;
    struct no_merge_point_to_end_print_method;
    struct point_to_end_print_method;
} // namespace interval::print_policy

namespace interval {
    template <typename T, detail::type_policy_c type_policy = type_policy::standard_type_policy>
    class interval {
        using inner_type = std::pair<int, T>;

    public:
        /// type of minimal obj
        struct minimal_t {
            [[nodiscard]] static std::pair<int, T> data() noexcept;
        };
        /// type of maximal obj
        struct maximal_t {
            [[nodiscard]] static std::pair<int, T> data() noexcept;
        };

        /// return always minimal element in any interval
        [[nodiscard]] minimal_t minimal() const noexcept {
            (void)this;
            return {};
        }
        /// return always maximal element in any interval
        [[nodiscard]] maximal_t maximal() const noexcept {
            (void)this;
            return {};
        }

        // structure can receive data with minimal<T>, maximal<T>, T
        using inp_type = std::variant<minimal_t, maximal_t, T>;
        interval();
        ~interval();

        interval(const interval &other);
        interval(interval &&other) noexcept;

        interval &operator=(const interval &other);
        interval &operator=(interval &&other) noexcept;

        [[nodiscard]] bool operator==(const interval &b) const noexcept;
        [[nodiscard]] bool operator!=(const interval &b) const noexcept;

        // standard operations

        /// return true if this multitude is empty, else return false
        [[nodiscard]] bool empty() const noexcept;

        /// return true if this point in multitude, else return false
        [[nodiscard]] bool in(const T &a) const;
        /// return true if this point in multitude, else return false
        [[nodiscard]] bool in_v(const inp_type &a) const;

        /// returns false if this point was inside this multitude, else return true
        template <typename U>
        bool add_point(U &&a);

        [[nodiscard]] detail::temp_policy_wrapper<T, type_policy>
        with_policy(const detail::standard_policy &policy) const &;

        /// return string with all data in mathematical style
        [[nodiscard]] std::string to_string() const;

    protected:
        using points_t = std::set<inner_type, detail::pair_less<T>>;
        using intervals_t = std::set<std::pair<inner_type, inner_type>, detail::pair_less<T>>;
        points_t points;
        intervals_t intervals;

        /// convert T-type object to pair inner-type {1; T-type elem};
        /// if object is interval::minimal or interval::maximal, return their data
        [[nodiscard]] static constexpr inner_type to_point(inp_type a);

        /// does what it has to
        [[nodiscard]] auto get_interval_that_include_this_point(const inner_type &point) const
            -> intervals_t::const_iterator;
        /// does what it has to
        [[nodiscard]] auto get_interval_that_include_this_point(const T &point) const -> intervals_t::const_iterator;

        /// check that point is not `-INF` and `+INF`
        template <typename U>
        static void constexpr is_point_assert(const U &point);

        bool add_point_in(inner_type p);

    private:
        static void constexpr is_point_assert_in(const inp_type &point);

        void merge_intervals(const std::pair<inner_type, inner_type> &f, const std::pair<inner_type, inner_type> &s);

        static std::string to_string_symbol(const inner_type &a, const std::string &min, const std::string &max);

        [[nodiscard]] std::string to_string_in(
        const std::string &min,
        const std::string &max,
        const std::string &empty,
        const std::string &sep,
        const std::string &unite,
        int id) const;
    };
} // namespace interval

// default realisations

namespace interval::type_policy {
    struct standard_type_policy : detail::type_policy {
        template <typename T>
        static constexpr bool is_arithmetic_v = std::is_arithmetic_v<T>;
        template <typename T>
        static constexpr bool is_integral_v = std::is_integral_v<T>;
        template <typename T>
        static constexpr bool is_string_v = std::is_same_v<std::string, T>;
    };
} // namespace interval::type_policy
namespace interval::detail::custom_type {
    template <typename T, not_custom_type_policy_c>
    std::string to_str(const T el) {
        std::stringstream ss;
        ss << std::move(el);
        return ss.str();
    }
} // namespace interval::detail::custom_type
#endif // MATHINTERVAL_BASE_H
