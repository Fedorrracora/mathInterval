#ifndef interval_H
#define interval_H
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <functional>

namespace interval {

    /// return always minimal element in any interval
    template <typename T>
    struct minimal {
        [[nodiscard]] static std::pair<int, T> data() noexcept {return {0, {}};}
    };

    /// return always maximal element in any interval
    template <typename T>
    struct maximal {
        [[nodiscard]] static std::pair<int, T> data() noexcept {return {2, {}};}
    };

    template<typename T>
    class interval {
    public:
        using inner_type = std::pair<int, T>;
        // structure can receive data with minimal<T>, maximal<T>, T type and inner_type
        using inp_type = std::variant<minimal<T>, maximal<T>, T, inner_type>;

        interval() = default;
        ~interval() = default;

        interval(const interval &other): points(other.points), intervals(other.intervals) {}
        interval(interval &&other) noexcept: points(std::move(other.points)), intervals(std::move(other.intervals)) {}

        interval & operator=(const interval &other) {
            if (this == &other) return *this;
            points = other.points, intervals = other.intervals;
            return *this;
        }
        interval & operator=(interval &&other) noexcept {
            if (this == &other) return *this;
            points = std::move(other.points), intervals = std::move(other.intervals);
            return *this;
        }

        friend void swap(interval &lhs, interval &rhs) noexcept {
            using std::swap;
            swap(lhs.points, rhs.points);
            swap(lhs.intervals, rhs.intervals);
        }

        [[nodiscard]] bool operator==(const interval &b) const noexcept {
            return this == &b || (points == b.points && intervals == b.intervals);
        }
        [[nodiscard]] bool operator!=(const interval &b) const noexcept {
            return !(*this == b);
        }

        // standard operations

        /// returns false if this point was inside this multitude, else return true
        bool add_point(const inp_type &a) {return add_point_in(to_point(a));}
        /// returns false if this point was not inside this multitude, else return true
        bool remove_point(const inp_type &a) {return remove_point_in(to_point(a));}

        /// returns false if all this interval was inside this multitude, else return true
        bool add_interval(const inp_type &a, const inp_type &b)
        {return add_interval_in(to_point(a), to_point(b));}
        /// returns false if all this interval was not inside this multitude, else return true
        bool remove_interval(const inp_type &a, const inp_type &b)
        {return remove_interval_in(to_point(a), to_point(b));}

        /// return true if this multitude is empty, else return false
        [[nodiscard]] bool empty() const noexcept {return points.empty() && intervals.empty();}
        /// clear multitude data
        void clear() {points.clear(); intervals.clear();}

        /// return true if this point in multitude, else return false
        [[nodiscard]] bool in(const inp_type &a) const
        {return points.count(to_point(a)) || get_interval_that_include_this_point(to_point(a)) != intervals.end();}

        /// return true if interval (a, b) in multitude, else return false
        [[nodiscard]] bool in(const inp_type &a, const inp_type &b) const
        {return check_in(to_point(a), to_point(b));}

        /// return true if interval in multitude, else return false
        [[nodiscard]] bool in(const std::pair<inp_type, inp_type> &a) const {
            return check_in(to_point(a.first), to_point(a.second));
        }

        /// return true if this multitude is subset of another multitude, else return false
        [[nodiscard]] bool in(const interval &b) const {
            return *this * b == *this;
        }
        /// return true if this multitude is subset of another multitude, else return false
        [[nodiscard]] bool issubset(const interval &b) const {
            return this->in(b);
        }

        /// return true if another multitude is subset of this multitude, else return false
        [[nodiscard]] bool issuperset(const interval &b) const {
            return b.in(*this);
        }

        /// return true if these multitudes has no common points, else return false
        [[nodiscard]] bool isdisjoint(const interval &b) const {
            return (*this * b).empty();
        }

        /// return true if multitude has only separate points (or empty), else return false
        [[nodiscard]] bool points_only() const {
            return intervals.empty();
        }

        // math operations

        /// returns a new multitude containing the union of the elements of the previous multitudes
        [[nodiscard]] friend interval operator+(interval a, const interval &b) {
            a += b;
            return a;
        }
        /// returns a new multitude containing the union of the elements of the previous multitudes
        [[nodiscard]] friend interval operator|(const interval &a, const interval &b) {return a + b;}
        /// adds elements of another multitude
        interval &operator+=(const interval &b) {
            if (this == &b) return *this;
            for (auto &i : b.points) add_point_in(i);
            for (auto &i : b.intervals) add_interval_in(i.first, i.second);
            return *this;
        }
        /// adds elements of another multitude
        interval &operator|=(const interval &b) {return this->operator+=(b);}

        /// returns a new multitude containing the difference of the elements of the previous multitudes
        [[nodiscard]] friend interval operator-(interval a, const interval &b) {
            a -= b;
            return a;
        }
        /// remove elements of another multitude
        interval &operator-=(const interval &b) {
            if (this == &b) {
                clear();
                return *this;
            }
            for (auto &i : b.points) remove_point_in(i);
            for (auto &i : b.intervals) remove_interval_in(i.first, i.second);
            return *this;
        }

        /// returns a new multitude containing the intersection of the elements of the previous multitudes
        [[nodiscard]] friend interval operator*(const interval &a, const interval &b) {
            interval x, y, z;
            a.invert_in(x), b.invert_in(y);
            x += y;
            x.invert_in(z);
            return z;
        }
        /// returns a new multitude containing the intersection of the elements of the previous multitudes
        [[nodiscard]] friend interval operator&(const interval &a, const interval &b) {return a * b;}
        /// intersect elements with another multitude
        friend interval &operator*=(interval &a, const interval &b) {
            if (&a == &b) return a;
            a = a * b;
            return a;
        }
        /// intersect elements with another multitude
        friend interval &operator&=(interval &a, const interval &b) {return a *= b;}

        /// returns a new multitude containing the symmetric difference of the elements of the previous multitudes
        [[nodiscard]] friend interval operator^(const interval &a, const interval &b) {
            return a + b - a * b;
        }

        /// generating symmetric difference with elements of another multitude
        friend interval &operator^=(interval &a, const interval &b) {
            auto buf = a * b;
            a += b;
            return a -= buf;
        }



        // transfer operations

        /// returns a new multitude with the points shifted forward by the distance val
        [[nodiscard]] interval operator+(const T val) const requires std::is_arithmetic_v<T> {interval b; plus_in(b, val); return b;}
        /// shift the points forward by a distance of val
        friend interval & operator+=(interval &a, const T val)
                {interval b; a.plus_in(b, val); a = std::move(b); return a;}

        /// returns a new multitude with the points shifted backward by the distance val
        [[nodiscard]] interval operator-(const T val) const requires std::is_arithmetic_v<T> {interval b; plus_in(b, -val); return b;}
        /// shift the points backward by a distance of val
        friend interval & operator-=(interval &a, const T val) {
            interval b; a.plus_in(b, -val); a = std::move(b); return a; // because I use -val, in strings it is error
        }

        /// returns a new multitude with the points multiplied by a factor of val
        [[nodiscard]] interval operator*(const T val) const requires std::is_arithmetic_v<T> {interval b; multiply_in(b, val); return b;}
        /// multiplies the points of a multitude by a factor of val
        friend interval & operator*=(interval &a, const T val)
                {interval b; a.multiply_in(b, val); a = std::move(b); return a;}

        /// returns a new multitude with the points divided by a factor of val
        [[nodiscard]] interval operator/(const T val) const requires std::is_arithmetic_v<T> {interval b; division_in(b, val); return b;}
        /// divides the points of a multitude by a factor of val
        friend interval & operator/=(interval &a, const T val)
                {interval b; a.division_in(b, val); a = std::move(b); return a;}

        /// returns a new multitude with points taken as the remainder of the division by val
        [[nodiscard]] interval operator%(const T val) const requires std::is_integral_v<T> {interval b; remainder_in(b, val); return b;}
        /// replaces the points with the remainder of the division by val
        friend interval & operator%=(interval &a, const T val)
        {interval b; a.remainder_in(b, val); a = std::move(b); return a;}

        // advanced operations

        /// returns the multitude that is the inverse of the given one
        [[nodiscard]] interval inverse() const {
            interval buf;
            invert_in(buf);
            return buf;
        }

        /// return string with all data in mathematical style
        [[nodiscard]] std::string print() const {return print_in();}

        /**
        ### any

        Return any element that is in this multitude.

        The function returns `std::optional`, because the returning value does not always exist.

        ---

        - If there is any point, it will be returned.
        - If there is an interval `(-INF; +INF)`, the function will return `T{}`. If the flag is true, will return `std::nullopt`.
        - If `T` is an integer or a non-integer digit,
          a smart algorithm will try to find any number in the intervals.
        - If `T` is `std::string`,
          a smart algorithm will try to find any string in the intervals,
          considering that a string may contain only **capital English letters**.
        - If `T` is another type, or if the smart algorithm does not find any element in data,
          the function will return `std::nullopt`.

        ---

        For custom types and algorithms, consider using this function with additional arguments.
         */
        [[nodiscard]] std::optional<T> any(bool flag = false) const {
            if (!points.empty()) return points.begin()->second; // if there is any point: return it
            if (flag && in(minimal<T>(), maximal<T>())) {
                return std::nullopt;
            }
            return get_any_in(intervals); // try to return something in intervals
        }

        /**
        ### any()

        Return any element that is in data.

        The function returns `std::optional`, because the returning value does not always exist.

        This function takes **three lambda functions**, each of which must return `std::optional<T>`,
        and **one `T` value**:

        ---

        - **First lambda** – called if there is an interval `(-INF; x)`,
          receives one `const T&` argument.
        - **Second lambda** – called if there is an interval `(x; +INF)`,
          receives one `const T&` argument.
        - **Third lambda** – called if there is an interval `(x; y)`,
          receives two `const T&` arguments.
        - **Value** - result for interval `(-INF, +INF)`

        ---

        Because the lambdas return `std::optional`, a lambda may return `std::nullopt`
        if the interval has no integer value.

        ⚠️ **Warning:**
        You must yourself detect that the returning value lies inside the interval.
        Do not forget to monitor for overflows.
         */
        [[nodiscard]] std::optional<T> any(
            const std::function<std::optional<T>(const T&)> &MINUS_INF_x,
            const std::function<std::optional<T>(const T&)> &x_PLUS_INF,
            const std::function<std::optional<T>(const T&, const T&)> &x_y, const T MINUS_INF_PLUS_INF) const {
            if (!points.empty()) return points.begin()->second; // if there is any point: return it
            return get_any_functional(intervals, MINUS_INF_x, x_PLUS_INF, x_y, MINUS_INF_PLUS_INF);
        }

        /**
        ### custom_transfer()

        Transfer all elements that are in this multitude and return a new `interval::interval`.

        The function takes **one lambda function**, that must return `T`.
        `-INF` and `+INF` remain unchanged.

        ---

        - **Lambda** – returns a new value of a point/border of an interval,
          receives one `const T&` argument.

        ---

        If the first value of an interval becomes greater than the second,
        the function will swap them automatically.
         */
        [[nodiscard]] interval custom_transfer(const std::function<T(const T&)> &fun) const {
            interval b;
            custom_transfer_in(b, fun);
            return b;
        }
        /**
        ### custom_transfer()

        Transfer all elements that are in this multitude and return a new `interval::interval`.

        The function takes **one lambda function**, that must return `T`,
        and **two values** – the converted values of `-INF` and `+INF`.
        New values cannot themselves be `-INF` or `+INF`.

        ---

        - **Lambda** – returns a new value of a point/border of an interval,
          receives one `const T&` argument.
        - **First value** – new value of the border of the interval, that begins from `-INF`.
        - **Second value** – new value of the border of the interval, that ends with `+INF`.

        ---

        If the first value of an interval becomes greater than the second,
        the function will swap them automatically.
         */
        [[nodiscard]] interval custom_transfer(const std::function<T(const T&)> &fun,
                                               const T& MINUS_INF, const T& PLUS_INF) const {
            interval b;
            custom_transfer_in(b, fun, MINUS_INF, PLUS_INF);
            return b;
        }
        /**
        ### _custom_transfer()

        Transfer all elements that are in data and return a new `interval::interval`.

        ⚠️ **Warning:**
        This function is intended to work with **raw memory**.

        The function takes **two lambda functions**:

        ---

        - **First lambda** – returns a new value of an interval in raw view
          (`std::pair<inner_type, inner_type>`),
          receives one `const std::pair<inner_type, inner_type>&` argument
          (the last value of the interval).
        - **Second lambda** – returns a new value of a point in normal view,
          receives one `const T&` argument (the last value of the point).

        ---

        A point cannot be `-INF` or `+INF`, so it is processed in the standard way.

        Here `inner_type` is `std::pair<int, T>`.
        Meaning of the **first element** of `inner_type`:

        | Value | Meaning | Second element |

        |------:|--------|-----------------|

        | `0`   | `-INF` | becomes `T{}` automatically |

        | `1`   | standard value | holds the necessary value |

        | `2`   | `+INF` | becomes `T{}` automatically |

        | other | **undefined behavior** → throws `std::range_error` |

        Additional conditions:

        ---

        - If the first value of an interval becomes **greater** than the second,
          an `std::overflow_error` is thrown automatically (invalid interval).
        - If the first value of an interval is **equal** to the second,
          this interval will **not** be added.
         */
        [[nodiscard]] interval _custom_transfer(
                const std::function<std::pair<inner_type, inner_type>
                        (const inner_type&, const inner_type&)> &L_R,
                const std::function<T(const T&)> &POINT) const {
            interval b;
            custom_transfer_in(b, L_R, POINT);
            return b;
        }

    protected:

        struct pair_less {
            bool operator()(const inner_type& a, const inner_type& b) const noexcept {
                if (a.first != b.first) return a.first < b.first;
                if (a.first != 1) return false;
                return std::less<T>()(a.second, b.second);
            }
            bool operator()(const std::pair<inner_type, inner_type>& a, const std::pair<inner_type, inner_type>& b) const noexcept {
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
        };

        std::set<inner_type, pair_less> points;
        std::set<std::pair<inner_type, inner_type>, pair_less> intervals;

        /// convert T-type object to pair inner-type {1; T-type elem};
        /// if object already have inner_type, do nothing
        /// if object is interval::minimal or interval::maximal, return their data
        [[nodiscard]] static constexpr inner_type to_point(const inp_type &a) {
            auto y = std::get_if<minimal<T>>(&a);
            if (y != nullptr) return y->data();
            auto z = std::get_if<maximal<T>>(&a);
            if (z != nullptr) return z->data();
            auto x = std::get_if<T>(&a);
            if (x != nullptr) return {1, std::get<T>(a)};
            return std::get<inner_type>(a);
        }

        /// convert second index among -INF and +INF to {}
        [[nodiscard]] static inner_type recover(const inner_type &a) {
            if (a.first < 0 || a.first > 2) throw std::range_error("point or border of interval has undefined value");
            if (a.first != 1) return {a.first, {}};
            return a;
        }

        bool add_point_in(const inner_type &p) {
            if (p.first != 1) return false; // you cannot add points -INF and +INF

            // (x1; a); (a; x2) + {a} = (x1; x2)
            auto x = intervals.lower_bound({p, {}});
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
            if (points.count(p)) {
                return false;
            }

            // something + {a} = something + {a} if {a} not in something
            points.insert(p);
            return true;
        }

        bool remove_point_in(const inner_type &p) {
            // (x1; x2) - {a} = (x1; a); (a; x2) if {a} in (x1, x2)
            auto x = get_interval_that_include_this_point(p);
            if (x != intervals.end()) {
                split_interval(*x, p);
                return true;
            }

            // (something + {a}) - {a} = something
            if (points.count(p)) {
                points.erase(p);
                return true;
            }
            return false;
        }

        bool add_interval_in(inner_type f, inner_type s) {
            // (a; a) == empty
            if (f == s) return false;
            if (pair_less()(s, f)) throw std::logic_error("right border of interval is less than left");
            auto x = get_interval_that_include_this_point(f), y = get_interval_that_include_this_point(s);
            if (x == y && x != intervals.end()) return false;
            // (x1; x2); (x3; x4) + (f; s) = (x1; x2); (x3; x4) + (x1; s) if {f} in (x1; x2)
            if (x != intervals.end()) f = x->first;
            if (y != intervals.end()) s = y->second;

            // (f; x1); (x2; x3); (x4; s) + (f; s) = empty + (f; s)
            for (auto it = intervals.lower_bound({f, {}}); it != intervals.end() && it->first < s;) {
                auto old = it++;
                intervals.erase(old);
            }

            // something + (f; s) = something + (f; s) if (f; s) not in something
            intervals.emplace(f, s);

            // (f; s) + {s} + (s; x) = (f; x)
            x = intervals.lower_bound({f, s});
            y = x;
            ++y;
            if (y != intervals.end()) {
                if (y->first == s && points.count(s)) {
                    merge_intervals(*x, *y);
                    points.erase(s);
                }
            }
            // (x; f) + {f} + (f; s) = (x; s)
            x = intervals.lower_bound({f, s});
            if (x != intervals.begin()) {
                y = x;
                --y;
                if (y->second == f && points.count(f)) {
                    merge_intervals(*y, *x);
                    points.erase(f);
                }
            }

            // (f; s) + {x} = (f; s) if x in (f; s)
            for (auto it = points.upper_bound(f); it != points.end() && *it < s;) {
                auto old = it++;
                points.erase(old);
            }
            return true;
        }

        bool remove_interval_in(const inner_type &f, const inner_type &s) {
            // (a; a) == empty
            if (f == s) return false;
            if (pair_less()(s, f)) throw std::logic_error("right border of interval is less than left");
            auto x = get_interval_that_include_this_point(f), y = get_interval_that_include_this_point(s);
            bool ret = false;

            // (x1; x2) - (f; s) = (x1; f] + [s; x2) if f, s in (x1; x2)
            if (x != intervals.end() && x == y) {
                points.insert(f);
                points.insert(s);
                intervals.emplace(x->first, f);
                intervals.emplace(s, x->second);
                intervals.erase(x);
                ret = true;
                goto point;
            }

            // (x1; x2) - (f; s) + something = (x1; f] - (f; s) + something if f in (x1; x2)
            if (x != intervals.end()) {
                intervals.emplace(x->first, f);
                points.insert(f);
                intervals.erase(x);
                ret = true;
            }
            // (x1; x2) - (f; s) + something = [s; x2) - (f; s) + something if s in (x1; x2)
            if (y != intervals.end()) {
                intervals.emplace(s, y->second);
                points.insert(s);
                intervals.erase(y);
                ret = true;
            }

            point:
            // something + (x1; x2) - (f; s) = something - (f; s) if (x1; x2) in (f; s)
            for (auto it = intervals.lower_bound({f, {}}); it != intervals.end() && it->first < s;) {
                auto old = it++;
                intervals.erase(old);
                ret = true;
            }
            // something + {x} - (f; s) = something - (f; s) if {x} in (f; s)
            for (auto it = points.upper_bound(f); it != points.end() && *it < s;) {
                auto old = it++;
                points.erase(old);
            }
            return ret;
        }

        void invert_in(interval &buf) const {
            buf.add_interval_in(minimal<T>().data(), maximal<T>().data());
            for (auto &it : intervals) {
                buf.remove_interval(it.first, it.second);
            }
            for (auto &it : points) buf.remove_point(it);
        }

        void plus_in(interval &buf, const T &val) const requires std::is_arithmetic_v<T> {
            // if point was -INF or +INF, recover will return second elem to {}
            for (auto &[fst, snd] : intervals) {
                buf.intervals.emplace(recover(std::make_pair(fst.first, fst.second + val)),
                                      recover(std::make_pair(snd.first, snd.second + val)));
            }
            for (auto &i : points) {
                buf.points.insert(recover(std::make_pair(i.first, i.second + val)));
            }
        }

        void multiply_in(interval &buf, const T &val) const requires std::is_arithmetic_v<T> {
            // if point was -INF or +INF second elem if set to 0 -> 0 * val = 0
            if (val == 0) {
                if (!intervals.empty() || !points.empty()) {
                    buf.clear();
                    buf.points.emplace(1, T{});
                }
                else buf.clear();
                return;
            }
            for (auto &[fst, snd] : intervals) {
                auto a = std::make_pair(val < 0 ? 2 - fst.first:fst.first, fst.second * val),
                     b = std::make_pair(val < 0 ? 2 - snd.first:snd.first, snd.second * val);
                // if val < 0: fst < snd -> a > b
                if (a != b) buf.intervals.emplace(std::min(a, b), std::max(a, b));
            }
            for (auto &i : points) {
                buf.points.insert(std::make_pair(i.first, i.second * val));
            }
        }

        void division_in(interval &buf, const T &val) const requires std::is_arithmetic_v<T> {
            // if point was -INF or +INF, second elem if set to 0 -> 0 / val = 0
            for (auto &[fst, snd] : intervals) {
                auto a = std::make_pair(val < 0 ? 2 - fst.first:fst.first, fst.second / val),
                     b = std::make_pair(val < 0 ? 2 - snd.first:snd.first, snd.second / val);
                // if val < 0: fst < snd -> a > b
                if (a != b) buf.intervals.emplace(std::min(a, b), std::max(a, b));
            }
            for (auto &i : points) {
                buf.points.insert(std::make_pair(i.first, i.second / val));
            }
        }

        void remainder_in(interval &buf, const T &val) const requires std::is_integral_v<T> {
            if (val <= 0) throw std::logic_error("the coefficient of the remainder of the division <= 0");
            for (auto &[fst, snd] : intervals) {
                auto len = snd.second - fst.second;
                // fst.first != 1 || snd.first != 1 -> -INF or +INF -> there is interval with len > val
                if (fst.first != 1 || snd.first != 1 || len > val) {
                    // [0, val)
                    buf.add_point(T{});
                    buf.add_interval(T{}, val);
                    break;
                }
                auto r = fst.second % val + len; // l of interval is fst.second % val
                if (r > val) { // l in [0, val); l + len not in [0, val) -> (l, val) U [0, (l + len) % val)
                    buf.add_interval(fst.second % val, val);
                    buf.add_point(T{});
                    buf.add_interval(T{}, r % val);
                }
                else buf.add_interval(fst.second % val, r); // l and l + len in [0, val) -> (l, l + len)
            }
            for (auto &i : points) {
                buf.add_point(i.second % val);
            }
        }

        void custom_transfer_in(interval &buf, const std::function<T(const T&)> &fun) const {
            // if point was -INF or +INF, recover will return second elem to {}
            for (auto &[fst, snd] : intervals) {
                auto x = recover(std::make_pair(fst.first, fst.first == 1 ? fun(fst.second):fst.second));
                auto y = recover(std::make_pair(snd.first, snd.first == 1 ? fun(snd.second):snd.second));
                buf.add_interval_in(std::min(x, y), std::max(x, y));
            }
            for (auto &i : points) {
                buf.add_point_in(recover(std::make_pair(i.first, i.first == 1 ? fun(i.second):i.second)));
            }
        }

        void custom_transfer_in(interval &buf, const std::function<T(const T&)> &fun, const T& a, const T& b) const {
            for (auto &[fst, snd] : intervals) {
                auto x = (fst.first != 1 ? std::make_pair(1, a):std::make_pair(fst.first, fun(fst.second)));
                auto y = (snd.first != 1 ? std::make_pair(1, b):std::make_pair(snd.first, fun(snd.second)));
                buf.add_interval_in(std::min(x, y), std::max(x, y));
            }
            for (auto &i : points) {
                buf.add_point_in(recover(std::make_pair(i.first, i.first == 1 ? fun(i.second):i.second)));
            }
        }
        void custom_transfer_in(interval &buf,
                                const std::function<std::pair<inner_type, inner_type>
                                        (const inner_type&, const inner_type&)> &L_R,
                                const std::function<T(const T&)> &POINT) const {
            for (auto &[fst, snd] : intervals) {
                auto x = L_R(fst, snd);
                x.first = recover(x.first);
                x.second = recover(x.second);
                if (x.first == x.second) continue;
                if (x.second < x.first) throw std::overflow_error("right border of interval is less than left");
                buf.add_interval_in(x.first, x.second);
            }
            for (auto &i : points) {
                buf.add_point_in(recover(std::make_pair(i.first, POINT(i.second))));
            }
        }

        [[nodiscard]] bool check_in(const inner_type &a, const inner_type &b) const {
            if (pair_less()(b, a)) throw std::logic_error("right border of interval is less than left");
            if (a == b) return true;
            auto x = intervals.upper_bound(std::make_pair(a, maximal<T>().data()));
            if (x == intervals.begin()) return false;
            --x;
            if (!pair_less()(a, x->first) && !pair_less()(x->second, b)) {
                return true;
            }
            return false;
        }
    private:
        [[nodiscard]] std::string print_in() const {
            auto point_iter = points.begin();
            auto interval_iter = intervals.begin();
            std::string out;

            // convert (0; T-type elem) to "-INF" and (2; T-type elem) to "+INF". (1; T-type elem) prints normally
            auto data = [](const inner_type &p)->std::string {
                switch (p.first) {
                    case 0:
                        return "-INF";
                    case 2:
                        return "+INF";
                    default:
                        return spec_to_string(p.second);
                }
            };

            // check empty data
            if (empty()) {
                out += "*Empty*";
                return out;
            }

            // create a buffer to convert {x1}; {x2} to {x1; x2} and (x1; x2) + {x1} to [x1; x2)
            std::vector<typename std::set<inner_type, pair_less>::iterator> point_buffer;

            // print a buffer to "{x1; x2; x3; ...; xk}" and clear buffer
            auto print_buffer = [&data, &point_buffer, &out] {
                if (!point_buffer.empty()) {
                    out += "{" + data(*point_buffer.front());
                    for (auto i = 1; i < point_buffer.size(); ++i) {
                        out += "; " + data(*point_buffer[i]);
                    }
                    out += "} U ";
                }
                point_buffer.clear();
            };

            while (point_iter != points.end() || interval_iter != intervals.end()) {
                // if point data less than begin of next interval or intervals ends
                if (interval_iter == intervals.end() ||
                    (point_iter != points.end() && *point_iter <= interval_iter->first)) {
                    // add point to buffer
                    point_buffer.push_back(point_iter);
                    ++point_iter;
                }
                else {
                    // b1 - begin from '['; b2 - end with ']'
                    bool b1 = false, b2 = false;

                    // (x1; x2) + {x1} = [x1; x2)
                    if (!point_buffer.empty()) {
                        if (*point_buffer.back() == interval_iter->first) {
                            point_buffer.pop_back();
                            b1 = true;
                        }
                    }

                    print_buffer();

                    // if next point = x2; (x1; x2) + {x2} = (x1; x2]
                    if (point_iter != points.end() && *point_iter == interval_iter->second) {
                        ++point_iter;
                        b2 = true;
                    }
                    out += (b1 ? '[':'(') + data(interval_iter->first) + "; " +
                           data(interval_iter->second) + (b2 ? ']':')') + " U ";
                    ++interval_iter;
                }
            }

            print_buffer();
            out.pop_back();
            out.pop_back();
            out.pop_back();
            return out;
        }


        [[nodiscard]] static std::string spec_to_string(const T &a) requires (!std::is_same_v<std::string, T>) {
            std::stringstream ss; // emulate standard output
            ss << a;
            return ss.str();
        }
        [[nodiscard]] static std::string spec_to_string(const std::string &a) {
            return "\"" + a + "\""; // for strings
        }

        [[nodiscard]] std::set<std::pair<inner_type, inner_type>, pair_less>::iterator
        get_interval_that_include_this_point(const inner_type &point) const {
            auto x = intervals.upper_bound({point, maximal<T>().data()}); // get interval (>point; x2)
            if (x != intervals.begin()) {
                --x; // interval (x1; <=point)
                if (pair_less()(point, x->second) && pair_less()(x->first, point)) return x;
            }
            return intervals.end();
        }

        // (x1; x2) + (x2; x3) + {x2} = (x1; x3)
        void merge_intervals(const std::pair<inner_type, inner_type> &f, const std::pair<inner_type, inner_type> &s) {
            intervals.emplace(f.first, s.second);
            intervals.erase(f);
            intervals.erase(s);
        }

        // (x1; x3) - {x2} = (x1; x2) + (x2; x3) if {x2} in (x1; x3)
        void split_interval(const std::pair<inner_type, inner_type> &f, const inner_type &s) {
            intervals.emplace(f.first, s);
            intervals.emplace(s, f.second);
            intervals.erase(f);
        }

        [[nodiscard]] std::optional<T> get_any_in // for degrees
                (const std::set<std::pair<std::pair<int, T>, std::pair<int, T>>, pair_less> &a) const requires std::is_arithmetic_v<T> {
            for (auto &i : a) {
                if (i.first.first == 0 && i.second.first == 2) return {}; // (-INF; +INF) -> 0
                // i.second.second - 1 < i.second.second and i.first.second + 1 > i.first.second
                // are need for detect overflow
                if (i.first.first == 0 && i.second.first == 1 && i.second.second - 1 < i.second.second)
                    return i.second.second - 1; // (-INF; x) -> x - 1
                if (i.first.first == 1 && i.second.first == 2 && i.first.second + 1 > i.first.second)
                    return i.first.second + 1; // (x; +INF) -> x + 1
                if (i.first.first == 1 && i.second.first == 1) { // (x1; x2) -> (x1 + x2) / 2
                    auto x = (i.second.second - i.first.second) / 2 + i.first.second;
                    if (x > i.first.second && x < i.second.second) return x;
                }
            }
            return std::nullopt;
        }

        [[nodiscard]] static std::optional<std::string> get_any_in // for string
                (const std::set<std::pair<std::pair<int, std::string>, std::pair<int, std::string>>, pair_less> &a)
                    requires std::is_same_v<std::string, T> {
            for (const auto &[fst, snd] : a) {
                if (fst.first == 0 && snd.first == 2) return "aboba"; // (-INF; +INF) -> "aboba"
                if (fst.first == 0 && snd.first == 1 && snd.second > "a") return "a";
                // (-INF; x) -> "a"
                if (fst.first == 1 && snd.first == 2) return 'z' + fst.second;
                // (x; +INF) -> "z" + x (> x)
                if (fst.first == 1 && snd.first == 1) { // (x1; x2) -> x1 + "a"
                    auto x = fst.second;
                    x += 'a';
                    if (x > fst.second && x < snd.second) return x;
                }
            }
            return std::nullopt;
        }

        [[nodiscard]] static std::optional<T> get_any_in // non-number types
        (const std::set<std::pair<std::pair<int, T>, std::pair<int, T>>, pair_less> &a)
                    requires (!std::is_same_v<std::string, T> and !std::is_arithmetic_v<T>) {
            if (a.size() == 1 && a.begin()->first.first == 0 && a.begin()->second.first == 2) return T{};
            return std::nullopt; // for unknown type I don`t know what I need to do
        }

        [[nodiscard]] static std::optional<T> get_any_functional
                (const std::set<std::pair<std::pair<int, T>, std::pair<int, T>>, pair_less> &a,
                const std::function<std::optional<T>(const T&)> &MINUS_INF_x,
                const std::function<std::optional<T>(const T&)> &x_PLUS_INF,
                const std::function<std::optional<T>(const T&, const T&)> &x_y, const T& MINUS_INF_PLUS_INF) {
            for (auto &i : a) {
                if (i.first.first == 0 && i.second.first == 2) return MINUS_INF_PLUS_INF; // (-INF; +INF)
                if (i.first.first == 0 && i.second.first == 1)
                    if (auto x = MINUS_INF_x(i.second.second); x.has_value()) return x; // (-INF; x)
                if (i.first.first == 1 && i.second.first == 2)
                    if (auto x = x_PLUS_INF(i.first.second); x.has_value()) return x; // (x; +INF)
                if (i.first.first == 1 && i.second.first == 1) { // (x1; x2)
                    if (auto x = x_y(i.first.second, i.second.second); x.has_value()) return x; // (x1; x2)
                }
            }
            return std::nullopt;
        }
    };

} // interval

// template class interval::interval<int8_t>;
// template class interval::interval<int16_t>;
// template class interval::interval<int32_t>;
// template class interval::interval<int64_t>;
// template class interval::interval<u_int8_t>;
// template class interval::interval<u_int16_t>;
//template class interval::interval<u_int32_t>;
//template class interval::interval<u_int64_t>;
//template class interval::interval<float>;
//template class interval::interval<double>;
//template class interval::interval<long double>;
//template class interval::interval<std::string>;

#endif //interval_H
