#include <verifier.h>
#include <fstream>

namespace verify {
    namespace {
        /// return true if this char can be decoded as space
        [[nodiscard]] constexpr bool space_skip(const char c) {
            return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
        }
    }

    std::string read_file(const std::string &filename) {
        std::ifstream input(filename);
        if (!input.is_open()) {
            throw std::runtime_error("Could not open file " + filename);
        }
        std::string contents, tmp;
        while (std::getline(input, tmp)) {
            contents += tmp;
            contents += '\n';
        }
        input.close();
        return contents;
    }

    bool same(const std::string& a, const std::string& b, const bool case_sensitive, const bool whitespace_sensitive, const bool boundary_sensitive) {
        std::size_t bgnA = 0, bgnB = 0, endA = a.size(), endB = b.size();
        if (!boundary_sensitive) {
            while (bgnA != endA && space_skip(a[bgnA    ])) ++bgnA;
            while (bgnB != endB && space_skip(b[bgnB    ])) ++bgnB;
            while (bgnA != endA && space_skip(a[endA - 1])) --endA;
            while (bgnB != endB && space_skip(b[endB - 1])) --endB;
        }
        while (bgnA != endA && bgnB != endB) {
            if (!whitespace_sensitive) {
                if (const bool ba = space_skip(a[bgnA]), bb = space_skip(b[bgnB]); ba || bb) {
                    if (ba != bb) return false;
                    while (bgnA != endA && space_skip(a[bgnA])) ++bgnA;
                    while (bgnB != endB && space_skip(b[bgnB])) ++bgnB;
                    continue;
                }
            }
            if (case_sensitive) {
                if (a[bgnA] != b[bgnB]) return false;
            }
            else
                if (std::tolower(a[bgnA]) != std::tolower(b[bgnB])) return false;
            ++bgnA;
            ++bgnB;
        }
        return bgnA == endA && bgnB == endB;
    }

    line_checker::line_checker(std::string s) : last(last_str) {
        contain.str(std::move(s));
    }
    std::string line_checker::operator()() {
        if (!std::getline(contain, last_str)) throw std::runtime_error("Could not read line");
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
    double time_checker::time() const {
        return len;
    }

    int random_int_distribution(const int from, const int to) {
        if (from == to) return from;
        std::uniform_int_distribution dist(from, to);
        return dist(rnd);
    }
}
