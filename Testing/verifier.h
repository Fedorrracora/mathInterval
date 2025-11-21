#ifndef MATHINTERVAL_VERIFIER_H
#define MATHINTERVAL_VERIFIER_H
#include <string>

namespace verify {
    bool same(const std::string &a, const std::string &b, bool case_sensitive=false, bool boundary_sensitive=false, bool whitespace_sensitive=false);
}
#endif //MATHINTERVAL_VERIFIER_H