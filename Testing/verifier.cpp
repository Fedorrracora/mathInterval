#include "verifier.h"
namespace verify {
    namespace {
        [[nodiscard]] constexpr bool space_skip(const char c) {
            return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
        }
    }
    bool same(const std::string& a, const std::string& b, const bool case_sensitive, const bool boundary_sensitive, const bool whitespace_sensitive) {
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
}