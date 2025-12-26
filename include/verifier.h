#ifndef VERIFIER_H
#define VERIFIER_H
#include <chrono>
#include <optional>
#include <random>
#include <sstream>
#include <string>

namespace verify {
    /// return string with content of file
    std::string read_file(const std::string &filename);

    /**
     return true if two strings are equal with some filters

     - case_sensitive - does what it should
     - whitespace_sensitive - repetitions of spaces, enter and special characters decodes such one space
     - boundary_sensitive - remove all spaces, enters and special characters from begin and end of the string
     */
    bool same(const std::string &a, const std::string &b, bool case_sensitive = false,
              bool whitespace_sensitive = false, bool boundary_sensitive = false);

    /// helps to read a long string by lines
    struct line_checker {
        /// last read line
        const std::string &last;
        explicit line_checker(std::string s);
        /// gets next line
        std::string operator()();
        /// gets next line. If you run out of data, std::nullopt is returned
        std::optional<std::string> get();

    private:
        std::stringstream contain;
        std::string last_str;
        const bool is_stdin;
    };

    /// helps keep track of time
    struct time_checker {
        /// start timing
        void start();
        /// stop timing. Not work if you did not start
        void stop();
        /// get calculated time
        [[nodiscard]] double time() const;

    private:
        std::chrono::steady_clock::time_point begin_point, end_point;
        double len{};
        bool started = false;
    };

    inline std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

    /// return random value between from and to
    int random_int_distribution(int from, int to);

    /// max= (a = std::max(a, b)). If a changes, return true
    template <typename T, typename U>
    [[maybe_unused]] constexpr bool maxof(T &a, U &&b) {
        auto &&value = std::forward<U>(b);
        return a < value && (a = value, true);
    }

    /// min= (a = std::min(a, b)). If a changes, return true
    template <typename T, typename U>
    [[maybe_unused]] constexpr bool minof(T &a, U &&b) {
        auto &&value = std::forward<U>(b);
        return a > value && (a = value, true);
    }

    /// finds the index of substring entry into the string
    [[nodiscard]] std::size_t find(const std::string &where, std::string what, char ch = '#');

    /// checks for the presence of a substring in the string
    [[nodiscard]] bool in(const std::string &where, const std::string &what, char ch = '#');

    /// helps to take into account the error when working with float numbers
    template <typename T>
    struct near {
        T value, accuracy;
        near(T value, T accuracy) : value(value), accuracy(accuracy) {}

        [[nodiscard]] constexpr bool operator==(const near &a) const {
            return std::abs(value - a.value) < accuracy + a.accuracy;
        }

        [[nodiscard]] constexpr bool operator<(const near &a) const { return value - a.value < accuracy + a.accuracy; }
        [[nodiscard]] constexpr bool operator>(const near &a) const { return accuracy + a.accuracy < value - a.value; }

        [[nodiscard]] constexpr bool operator<=(const near &a) const {
            return value - a.value <= accuracy + a.accuracy;
        }

        [[nodiscard]] constexpr bool operator>=(const near &a) const {
            return accuracy + a.accuracy <= value - a.value;
        }

        [[nodiscard]] constexpr bool operator!=(const near &a) const {
            return std::abs(value - a.value) >= accuracy + a.accuracy;
        }

        [[nodiscard]] constexpr bool operator==(const T &a) const { return std::abs(value - a) < accuracy; }
        [[nodiscard]] constexpr bool operator<(const T &a) const { return value - a < accuracy; }
        [[nodiscard]] constexpr bool operator>(const T &a) const { return accuracy < value - a; }
        [[nodiscard]] constexpr bool operator<=(const T &a) const { return value - a <= accuracy; }
        [[nodiscard]] constexpr bool operator>=(const T &a) const { return accuracy <= value - a; }
        [[nodiscard]] constexpr bool operator!=(const T &a) const { return std::abs(value - a) >= accuracy; }
    };

    /// Does what is written. v - skip able chars
    [[nodiscard]] bool can_cast_to_digit(const std::string &s, std::string_view v);

    /// Change substring `from` to string `to` in string `where`. Return new string, not changes `where`
    [[nodiscard]] std::string change(const std::string &where, std::string_view from, std::string_view to);

    /// get n`th word in string
    [[nodiscard]] std::string n_word(const std::string &where, std::size_t n);

    /// removes special symbols around word
    [[nodiscard]] std::string boundary(const std::string &where);

    /// removes spaces around word
    [[nodiscard]] std::string boundary_spaces(const std::string &where);

    /// check if `where` begins with `what`
    [[nodiscard]] bool begin_from(const std::string &where, std::string_view what);

    /// such python split
    [[nodiscard]] std::vector<std::string> split(const std::string &s, char delim = ' ');

    /// such python join
    [[nodiscard]] std::string join(std::string_view sep, const std::vector<std::string> &data);
} // namespace verify
#endif // VERIFIER_H
