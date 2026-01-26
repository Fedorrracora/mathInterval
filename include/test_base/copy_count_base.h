#ifndef MATHINTERVAL_COPY_COUNT_BASE_H
#define MATHINTERVAL_COPY_COUNT_BASE_H
#include <gtest/gtest.h>
#include <mathInterval/type_policy.h>
#include <mathInterval/interval.h>
#include <verifier.h>
#include <functional>
namespace copy_count {
    struct tp final : interval::type_policy::custom_type_policy<verify::copy_counter> {
        verify::copy_counter default_value() override {
            return 3;
        }
        std::string to_string(const verify::copy_counter &el) override {
            return std::to_string(el.value);
        }
    };
    using interval_t = interval::interval<verify::copy_counter, tp>;
    namespace detail {
        struct inner_type {
            std::string name;
            std::function<interval_t()> gen_interval;
            std::function<void(interval_t)> without_copy;
            std::size_t without_counter;
            std::function<void(interval_t)> with_copy;
            std::size_t with_counter;
            std::size_t priority;
            inner_type(std::string name, std::function<interval_t()> gen_interval,
                       const std::size_t without_counter, std::function<void(interval_t)> without_copy,
                       const std::size_t with_counter, std::function<void(interval_t)> with_copy) :
            name(std::move(name)), gen_interval(std::move(gen_interval)),
            without_copy(std::move(without_copy)), without_counter(without_counter),
            with_copy(std::move(with_copy)), with_counter(with_counter) {
                static std::size_t priority_counter = 0;
                priority = priority_counter++;
            }
            [[nodiscard]] std::pair<int, int> call() const {
                auto interval1 = gen_interval();
                auto interval2 = gen_interval();
                verify::copy_counter::call();
                without_copy(std::move(interval1));
                auto x = verify::copy_counter::call();
                with_copy(std::move(interval2));
                auto y = verify::copy_counter::call();
                return {x, y};
            }
        };
        inline std::vector<inner_type> data;
        [[nodiscard]] inline bool operator<(const inner_type &a, const inner_type &b) {
            return a.priority < b.priority;
        }
    }

}
#endif // MATHINTERVAL_COPY_COUNT_BASE_H
