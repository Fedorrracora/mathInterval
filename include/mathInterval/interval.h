#ifndef MATHINTERVAL_INTERVAL_H
#define MATHINTERVAL_INTERVAL_H
#include <mathInterval/detail/base.h>

namespace interval {
    // pair_less

    template <typename T>
    bool detail::pair_less<T>::operator()(const inner_type &a, const inner_type &b) const noexcept {
        if (a.first != b.first) return a.first < b.first;
        if (a.first != 1) return false;
        return pair_less()(a.second, b.second);
    }
    template <typename T>
    bool detail::pair_less<T>::operator()(const std::pair<inner_type, inner_type> &a,
                                          const std::pair<inner_type, inner_type> &b) const noexcept {
        if (a.first.first != b.first.first) return a.first.first < b.first.first;
        if (a.first.first != 1) {
            if (a.second.first != b.second.first) return a.second.first < b.second.first;
            if (a.second.first != 1) return false;
            return a.second.second < b.second.second;
        }
        if (!(a.first < b.first || b.first < a.first)) {
            if (a.second.first != b.second.first) return a.second.first < b.second.first;
            if (a.second.first != 1) return false;
            return a.second.second < b.second.second;
        }
        return a.first < b.first;
    }
    template <typename T>
    bool detail::pair_less<T>::operator()(const T &a, const T &b) const noexcept {
        return std::less<T>()(a, b);
    }

    template <typename T>
    bool detail::pair_less<T>::operator()(const std::pair<inner_type, inner_type> &a,
                                          const inner_type &b) const noexcept {
        return pair_less()(a.first, b);
    }
    template <typename T>
    bool detail::pair_less<T>::operator()(const inner_type &a,
                                          const std::pair<inner_type, inner_type> &b) const noexcept {
        return pair_less()(a, b.first);
    }

    template <typename T>
    bool detail::pair_less<T>::operator()(const T &a, const inner_type &b) const noexcept {
        if (b.first != 1) return b.first == 0;
        return pair_less()(a, b.second);
    }
    template <typename T>
    bool detail::pair_less<T>::operator()(const inner_type &a, const T &b) const noexcept {
        if (a.first != 1) return a.first == 0;
        return pair_less()(a.second, b);
    }

    template <typename T>
    bool detail::pair_less<T>::operator()(const T &a, const std::pair<inner_type, inner_type> &b) const noexcept {
        return pair_less()(a, b.first);
    }
    template <typename T>
    bool detail::pair_less<T>::operator()(const std::pair<inner_type, inner_type> &a, const T &b) const noexcept {
        return pair_less()(a.first, b);
    }

    // minmax
    template <typename T, detail::type_policy_c type_policy>
    std::pair<int, T> interval<T, type_policy>::minimal_t::data() noexcept { return {0, detail::custom_type::get_value<T, type_policy>()}; }
    template <typename T, detail::type_policy_c type_policy>
    std::pair<int, T> interval<T, type_policy>::maximal_t::data() noexcept { return {2, detail::custom_type::get_value<T, type_policy>()}; }

    // default zone

    template <typename T, detail::type_policy_c type_policy>
    interval<T, type_policy>::interval() = default;
    template <typename T, detail::type_policy_c type_policy>
    interval<T, type_policy>::~interval() = default;

    template <typename T, detail::type_policy_c type_policy>
    interval<T, type_policy>::interval(const interval &other) = default;
    template <typename T, detail::type_policy_c type_policy>
    interval<T, type_policy>::interval(interval &&other) noexcept = default;

    template <typename T, detail::type_policy_c type_policy>
    interval<T, type_policy> &interval<T, type_policy>::operator=(const interval &other) = default;
    template <typename T, detail::type_policy_c type_policy>
    interval<T, type_policy> &interval<T, type_policy>::operator=(interval &&other) noexcept = default;

    // equal

    template <typename T, detail::type_policy_c type_policy>
    bool interval<T, type_policy>::operator==(const interval &b) const noexcept {
        return this == &b || (points == b.points && intervals == b.intervals);
    }

    template <typename T, detail::type_policy_c type_policy>
    bool interval<T, type_policy>::operator!=(const interval &b) const noexcept { return !(*this == b); }

    // some technical details

    template <typename T, detail::type_policy_c type_policy>
    constexpr interval<T, type_policy>::inner_type interval<T, type_policy>::to_point(inp_type a)  {
        auto y = std::get_if<minimal_t>(&a);
        if (y != nullptr) return y->data();
        auto z = std::get_if<maximal_t>(&a);
        if (z != nullptr) return z->data();
        auto x = std::get_if<T>(&a);
        if (x != nullptr) return {1, std::move(std::get<T>(a))};
        throw std::bad_cast();
    }

    template <typename T, detail::type_policy_c type_policy>
    void interval<T, type_policy>::merge_intervals(const std::pair<inner_type, inner_type> &f,
                                                   const std::pair<inner_type, inner_type> &s) {
        intervals.emplace(f.first, s.second);
        intervals.erase(f);
        intervals.erase(s);
    }

    template <typename T, detail::type_policy_c type_policy>
    auto interval<T, type_policy>::get_interval_that_include_this_point(const inner_type &point) const ->
    intervals_t::const_iterator {
        if (point.first != 1) throw std::range_error("point has undefined value (-INF or +INF)");
        return get_interval_that_include_this_point(point.second);
    }

    template <typename T, detail::type_policy_c type_policy>
    auto interval<T, type_policy>::get_interval_that_include_this_point(const T &point) const ->
    intervals_t::const_iterator {
        auto x = intervals.upper_bound(point); // get interval (>point; x2)
        if (x != intervals.begin()) {
            --x; // interval (x1; <=point)
            if (detail::pair_less<T>()(point, x->second) && detail::pair_less<T>()(x->first, point)) return x;
        }
        return intervals.end();
    }

    template <typename T, detail::type_policy_c type_policy>
    template <typename U>
    constexpr void interval<T, type_policy>::is_point_assert(const U &point) {
        if constexpr (!std::is_same_v<T, U>) is_point_assert_in(point);
    }
    template <typename T, detail::type_policy_c type_policy>
    constexpr void interval<T, type_policy>::is_point_assert_in(const inp_type &point) {
        if (std::get_if<T>(&point) == nullptr) throw std::range_error("point has undefined value (-INF or +INF)");
    }


    // default operations

    template <typename T, detail::type_policy_c type_policy>
    template <typename U>
    bool interval<T, type_policy>::add_point(U &&a) {
        is_point_assert(a);
        if (in(a)) return false;
        return add_point_in(to_point(std::forward<U>(a)));
    }

    template <typename T, detail::type_policy_c type_policy>
    bool interval<T, type_policy>::add_point_in(inner_type p) {
        if (p.first != 1) throw std::range_error("point has undefined value (-INF or +INF)");
        // you cannot add points -INF and +INF

        // (x1; a); (a; x2) + {a} = (x1; x2)
        auto x = intervals.lower_bound(p);
        if (x != intervals.end() && x->first == p && x != intervals.begin()) {
            auto y = x;
            --y;
            if (y->second == p) {
                merge_intervals(*y, *x);
                return true;
            }
        }

        // (x1; x2) + {a} = (x1; x2) if a in (x1; x2)
        if (get_interval_that_include_this_point(p) != intervals.end()) return false;

        // {a} + {a} = {a}
        if (points.count(p)) { return false; }

        // something + {a} = something + {a} if {a} not in something
        points.insert(std::move(p));
        return true;
    }

    // empty

    template <typename T, detail::type_policy_c type_policy>
    bool interval<T, type_policy>::empty() const noexcept { return points.empty() && intervals.empty(); }


    // in

    template <typename T, detail::type_policy_c type_policy>
    bool interval<T, type_policy>::in(const T &a) const {
        return points.count(a) || get_interval_that_include_this_point(a) != intervals.end();
    }
    template <typename T, detail::type_policy_c type_policy>
    bool interval<T, type_policy>::in_v(const inp_type &a) const {
        is_point_assert(a);
        return in(std::get<T>(a));
    }

    // to string
    template <typename T, detail::type_policy_c type_policy>
    std::string interval<T, type_policy>::to_string_symbol(const inner_type &a, const std::string &min,
                                                           const std::string &max) {
        if (a.first == 1) [[likely]] {
            return detail::custom_type::to_str<T, type_policy>(a.second);
        }
        if (a.first == 0) return min;
        if (a.first == 2) return max;
        throw std::runtime_error("point or border of interval has undefined value");
    }


    template <typename T, detail::type_policy_c type_policy>
    std::string interval<T, type_policy>::to_string_in(
        const std::string &min,
        const std::string &max,
        const std::string &empty,
        const std::string &sep,
        const std::string &unite,
        const int id
        ) const {
        if (this->empty()) return empty;
        std::string s1, s2;

    }

    // with policy
    template <typename T, detail::type_policy_c type_policy>
    detail::temp_policy_wrapper<T, type_policy>
        interval<T, type_policy>::with_policy(const detail::standard_policy &policy) const & {
        return detail::temp_policy_wrapper(*this).with_policy(policy);
    }


} // namespace interval

// template class interval::interval<int8_t>;
// template class interval::interval<int16_t>;
// template class interval::interval<int32_t>;
// template class interval::interval<int64_t>;
// template class interval::interval<u_int8_t>;
// template class interval::interval<u_int16_t>;
// template class interval::interval<u_int32_t>;
// template class interval::interval<u_int64_t>;
// template class interval::interval<float>;
// template class interval::interval<double>;
// template class interval::interval<long double>;
// template class interval::interval<std::string>;

#endif // MATHINTERVAL_INTERVAL_H
