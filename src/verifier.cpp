#include <fstream>
#include <iostream>
#include <ranges>
#include <verifier.h>

namespace verify {
    namespace {
        /// return true if this char can be decoded as space
        [[nodiscard]] constexpr bool space_skip(const char c) {
            return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
        }

        /// return true if char must not skip
        [[nodiscard]] constexpr bool not_skip(const char ch) {
            return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ('0' <= ch && ch <= '9') || ch == '_';
        }
    } // namespace

    std::string read_file(const std::string &filename) {
        std::ifstream input(filename);
        if (!input.is_open()) { throw std::runtime_error("Could not open file " + filename); }
        std::string contents, tmp;
        while (std::getline(input, tmp)) {
            contents += tmp;
            contents += '\n';
        }
        input.close();
        return contents;
    }

    bool same(const std::string &a, const std::string &b, const bool case_sensitive, const bool whitespace_sensitive,
              const bool boundary_sensitive) {
        std::size_t bgnA = 0, bgnB = 0, endA = a.size(), endB = b.size();
        if (!boundary_sensitive) {
            while (bgnA != endA && space_skip(a[bgnA]))
                ++bgnA;
            while (bgnB != endB && space_skip(b[bgnB]))
                ++bgnB;
            while (bgnA != endA && space_skip(a[endA - 1]))
                --endA;
            while (bgnB != endB && space_skip(b[endB - 1]))
                --endB;
        }
        while (bgnA != endA && bgnB != endB) {
            if (!whitespace_sensitive) {
                if (const bool ba = space_skip(a[bgnA]), bb = space_skip(b[bgnB]); ba || bb) {
                    if (ba != bb) return false;
                    while (bgnA != endA && space_skip(a[bgnA]))
                        ++bgnA;
                    while (bgnB != endB && space_skip(b[bgnB]))
                        ++bgnB;
                    continue;
                }
            }
            if (case_sensitive) {
                if (a[bgnA] != b[bgnB]) return false;
            }
            else if (std::tolower(a[bgnA]) != std::tolower(b[bgnB]))
                return false;
            ++bgnA;
            ++bgnB;
        }
        return bgnA == endA && bgnB == endB;
    }

    line_checker::line_checker(std::string s) : last(last_str), is_stdin(s == "-") {
        if (!is_stdin) contain.str(std::move(s));
    }

    std::string line_checker::operator()() {
        if (is_stdin) {
            if (!std::getline(std::cin, last_str)) throw std::runtime_error("Could not read line");
        }
        else {
            if (!std::getline(contain, last_str)) throw std::runtime_error("Could not read line");
        }
        return last_str;
    }

    std::optional<std::string> line_checker::get() {
        if (is_stdin) {
            if (!std::getline(std::cin, last_str)) return std::nullopt;
        }
        else {
            if (!std::getline(contain, last_str)) return std::nullopt;
        }
        return last_str;
    }

    void time_checker::start() {
        if (started) return;
        begin_point = std::chrono::steady_clock::now();
        started = true;
    }

    void time_checker::stop() {
        if (!started) return;
        started = false;
        end_point = std::chrono::steady_clock::now();
        const std::chrono::duration<double> dur = end_point - begin_point;
        len = dur.count();
    }

    double time_checker::time() const { return len; }

    int random_int_distribution(const int from, const int to) {
        if (from == to) return from;
        std::uniform_int_distribution dist(from, to);
        return dist(rnd);
    }

    std::size_t find(const std::string &where, std::string what, const char ch) {
        const auto what_size = what.size();
        what += ch;
        what += where;
        std::vector<std::size_t> zf(what.size());
        for (long i = 1, l = 0, r = 0; i < what.size(); ++i) {
            zf[i] = std::max(0l, std::min(r - i, static_cast<long>(zf[i - l])));
            while (i + zf[i] < what.size() && what[zf[i]] == what[i + zf[i]])
                ++zf[i];
            if (maxof(r, i + zf[i])) l = i;
            if (zf[i] == what_size) return i - what_size - 1;
        }
        return -1;
    }

    bool in(const std::string &where, const std::string &what, const char ch) { return find(where, what, ch) != -1; }

    bool can_cast_to_digit(const std::string &s, const std::string_view v) {
        for (auto &i : s) {
            if ('0' <= i && i <= '9') continue;
            bool cont = false;
            for (auto &j : v)
                if (i == j) {
                    cont = true;
                    break;
                }
            if (!cont) return false;
        }
        return true;
    }

    std::string change(const std::string &where, const std::string_view from, const std::string_view to) {
        std::string out;
        for (std::size_t i = 0; i < where.size();) {
            if (where.substr(i, from.size()) == from) {
                out += to;
                i += from.size();
            }
            else {
                out += where[i];
                ++i;
            }
        }
        return out;
    }

    std::string n_word(const std::string &where, std::size_t n) {
        std::string out;
        bool sp = true;
        for (auto &i : where) {
            if (space_skip(i)) {
                if (!sp) {
                    sp = true;
                    --n;
                }
                continue;
            }
            sp = false;
            if (n == 0) out += i;
        }
        return out;
    }

    std::string boundary(const std::string &where) {
        std::size_t l = 0, len = where.size();
        while (len && !not_skip(where[l]))
            ++l, --len;
        while (len && !not_skip(where[l + len - 1]))
            --len;
        return where.substr(l, len);
    }

    std::string boundary_spaces(const std::string &where) {
        std::size_t l = 0, len = where.size();
        while (len && where[l] == ' ')
            ++l, --len;
        while (len && where[l + len - 1] == ' ')
            --len;
        return where.substr(l, len);
    }

    bool begin_from(const std::string &where, const std::string_view what) {
        return where.substr(0, what.size()) == what;
    }

    std::vector<std::string> split(const std::string &s, const char delim) {
        std::vector<std::string> elems(1);
        bool sp = true;
        for (auto &i : s) {
            if (i == delim) {
                if (!sp) {
                    sp = true;
                    elems.emplace_back();
                }
                continue;
            }
            sp = false;
            elems.back() += i;
        }
        if (elems.back().empty()) elems.pop_back();
        return elems;
    }

    std::string join(const std::string_view sep, const std::vector<std::string> &data) {
        if (data.empty()) return "";
        std::string out = data.front();
        for (auto i = 1; i < data.size(); ++i) {
            out += sep;
            out += data[i];
        }
        return out;
    }

    copy_counter::copy_counter(const int v) : value(v) {}
    copy_counter &copy_counter::operator=(const int v) {
        value = v;
        return *this;
    }
    copy_counter::copy_counter(const copy_counter &other) : value(other.value) { ++copy_count; }
    copy_counter &copy_counter::operator=(const copy_counter &other) {
        value = other.value;
        ++copy_count;
        return *this;
    }
    copy_counter::copy_counter(copy_counter &&other) noexcept = default;
    copy_counter &copy_counter::operator=(copy_counter &&other) noexcept = default;
    int copy_counter::call() {
        const int x = copy_count;
        copy_count = 0;
        return x;
    }

} // namespace verify
