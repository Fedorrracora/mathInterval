#ifndef MATHINTERVAL_TYPE_POLICY_H
#define MATHINTERVAL_TYPE_POLICY_H
#include <mathInterval/detail/base.h>
namespace interval::type_policy {
    // default type policies

    struct int_type_policy : detail::type_policy {
        template <typename>
        static constexpr bool is_arithmetic_v = true;
        template <typename>
        static constexpr bool is_integral_v = true;
        template <typename>
        static constexpr bool is_string_v = false;
    };

    struct float_type_policy : detail::type_policy {
        template <typename>
        static constexpr bool is_arithmetic_v = true;
        template <typename>
        static constexpr bool is_integral_v = false;
        template <typename>
        static constexpr bool is_string_v = false;
    };

    struct string_type_policy : detail::type_policy {
        template <typename>
        static constexpr bool is_arithmetic_v = false;
        template <typename>
        static constexpr bool is_integral_v = false;
        template <typename>
        static constexpr bool is_string_v = true;
    };

    // custom type policy.

    /// This structure allows you to control many of the object's variables.
    /// In its pure form, this structure cannot be used. Inheritance is mandatory
    template <typename T>
    struct custom_type_policy : detail::custom_type_policy_flag {
        virtual ~custom_type_policy() = default;

        /// It clarifies what you should use instead of T{}.
        /// The return value is used as a placeholder. Its value is never used.
        /// Useful if you don't have a standard constructor for the type you're using.
        virtual T default_value() = 0;
    };
}


namespace interval::detail::custom_type {
    template <typename T, custom_type_policy_c type_policy>
    T get_value() {
        return type_policy().default_value();
    }
}

#endif // MATHINTERVAL_TYPE_POLICY_H
