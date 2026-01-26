#ifndef MATHINTERVAL_STANDARD_POLICY_H
#define MATHINTERVAL_STANDARD_POLICY_H
#include <mathInterval/detail/base.h>
namespace interval::detail {
    template <typename T, type_policy_c type_policy>
    class temp_policy_wrapper {
    public:
        temp_policy_wrapper(const temp_policy_wrapper &) = delete;
        temp_policy_wrapper &operator=(const temp_policy_wrapper &) = delete;
        temp_policy_wrapper &operator=(temp_policy_wrapper &&) = delete;
        [[nodiscard]] temp_policy_wrapper with_policy(const standard_policy &policy) && {
            policies.emplace_back(policy.clone());
            return std::move(*this);
        }
        std::string to_string() && {
            return "abc";
        }
    private:
        temp_policy_wrapper(temp_policy_wrapper &&) = default;
        explicit temp_policy_wrapper(const interval<T, type_policy> &el) : ref(el) {}
        std::reference_wrapper<const interval<T, type_policy>> ref;
        std::vector<std::unique_ptr<standard_policy>> policies;
        friend class interval<T, type_policy>;
    };
}
#endif // MATHINTERVAL_STANDARD_POLICY_H
