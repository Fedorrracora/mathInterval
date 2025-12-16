#include <register.h>
#include <additional_test_tools.h>

/// The numbers written here are approximate. They are replaced every time they are executed
std::vector<std::vector<std::string>> vec = {
    {"function"},
    {},
    {"time of processing 1000 elems (1250 iterations)"},
    {"time of processing 1000 elems (one iteration)"},
    {"time of processing 10 elems (1250 iterations)"},
    {"time of processing 10 elems (one iteration)"},
    {"asymptotics coefficient"}
};
enum OUTPUT_LINES {
    FUNCTION,
    HEADER,
    FIRST_TIME,
    FIRST_TIME_ITERATION,
    SECOND_TIME,
    SECOND_TIME_ITERATION,
    ASYMPTOTICS_COEFFICIENT
};
enum LINE_CONFIG {
    LINE_FUNCTION,
    LINE_FIRST_TIME,
    LINE_FIRST_TIME_ITERATION,
    LINE_SECOND_TIME,
    LINE_SECOND_TIME_ITERATION,
    LINE_ASYMPTOTICS_COEFFICIENT
};
bool begin_reading = false, conf = false;
REGISTER(TIME TEST (inverse), s) {
    if (!s.has_value()) {
        auto out = verifier_tests::to_table(vec, HEADER);
        return out;
    }
    if (begin_reading) {
        std::stringstream ss(s.value());
        if (!conf) {
            conf = true;
            std::string iters, a1, a2;
            ss >> iters >> a1 >> a2;
            vec[FIRST_TIME].front() = "time of processing " + a1 + " elems (" + iters + " iterations)";
            vec[FIRST_TIME_ITERATION].front() = "time of processing " + a1 + " elems (one iteration)";
            vec[SECOND_TIME].front() = "time of processing " + a2 + " elems (" + iters + " iterations)";
            vec[SECOND_TIME_ITERATION].front() = "time of processing " + a2 + " elems (one iteration)";
        }
        else {
            std::vector<std::string> info;
            std::string l;
            while (ss >> l) {
                info.push_back(l);
            }
            vec[FUNCTION].push_back(info[LINE_FUNCTION]);
            vec[FIRST_TIME].push_back(info[LINE_FIRST_TIME]);
            vec[FIRST_TIME_ITERATION].push_back(info[LINE_FIRST_TIME_ITERATION]);
            vec[SECOND_TIME].push_back(info[LINE_SECOND_TIME]);
            vec[SECOND_TIME_ITERATION].push_back(info[LINE_SECOND_TIME_ITERATION]);
            vec[ASYMPTOTICS_COEFFICIENT].push_back(info[LINE_ASYMPTOTICS_COEFFICIENT]);
        }
    }
    if (verify::same(s.value(), "----- The following text is needed only for the formatter -----", false, true, false)) {
        begin_reading = true;
    }
    return std::nullopt;
}
