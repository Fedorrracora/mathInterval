#ifndef MATHINTERVAL_VERIFIER_H
#define MATHINTERVAL_VERIFIER_H
#include <string>
#include <sstream>

namespace verify {
    std::string read_file(const std::string &filename);
    bool same(const std::string &a, const std::string &b, bool case_sensitive=false, bool whitespace_sensitive=false, bool boundary_sensitive=false);
    struct line_checker {
        const std::string &last;
        explicit line_checker(std::string s);
        std::string operator()();
    private:
        std::stringstream contain;
        std::string last_str;
    };
}
#endif //MATHINTERVAL_VERIFIER_H