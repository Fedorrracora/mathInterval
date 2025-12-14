#include <register.h>
#include <additional_test_tools.h>

std::vector<std::vector<std::string>> vec = {
    {"function"},
    {},
    {"generating time"},
    {"time of 1000 elems"},
    {"time of 1000 elems (one iteration)"},
    {"time of 10 elems"},
    {"time of 10 elems (one iteration)"},
    {"asymptotics coefficient"}
};
enum OUTPUT_LINES {
    FUNCTION,
    HEADER,
    GENERATE_TIME,
    FIRST_TIME,
    FIRST_TIME_ITERATION,
    SECOND_TIME,
    SECOND_TIME_ITERATION,
    ASYMPTOTICS_COEFFICIENT
};
enum LINE_CONFIG {
    LINE_FUNCTION,
    LINE_GENERATE_TIME,
    LINE_FIRST_TIME,
    LINE_FIRST_TIME_ITERATION,
    LINE_SECOND_TIME,
    LINE_SECOND_TIME_ITERATION,
    LINE_ASYMPTOTICS_COEFFICIENT
};
bool begin_reading = false, conf = false;
REGISTER(TIME TEST (inverse), s) {
    if (!s.has_value()) {
        auto out = verifier_tests::to_table(vec, OUTPUT_LINES::HEADER);
        return out;
    }
    if (begin_reading) {
        std::stringstream ss(s.value());
        if (!conf) {
            conf = true;
            std::string a1, a2;
            ss >> a1 >> a2;
            vec[FIRST_TIME].front() = "time of " + a1 + " elems";
            vec[FIRST_TIME_ITERATION].front() = "time of " + a1 + " elems (one iteration)";
            vec[SECOND_TIME].front() = "time of " + a2 + " elems";
            vec[SECOND_TIME_ITERATION].front() = "time of " + a2 + " elems (one iteration)";
        }
        else {
            std::vector<std::string> info;
            std::string l;
            while (ss >> l) {
                info.push_back(l);
            }
            vec[FUNCTION].push_back(info[LINE_FUNCTION]);
            vec[GENERATE_TIME].push_back(info[LINE_GENERATE_TIME]);
            vec[FIRST_TIME].push_back(info[LINE_FIRST_TIME]);
            vec[FIRST_TIME_ITERATION].push_back(info[LINE_FIRST_TIME_ITERATION]);
            vec[SECOND_TIME].push_back(info[LINE_SECOND_TIME]);
            vec[SECOND_TIME_ITERATION].push_back(info[LINE_SECOND_TIME_ITERATION]);
            vec[ASYMPTOTICS_COEFFICIENT].push_back(info[LINE_ASYMPTOTICS_COEFFICIENT]);
        }
    }
    if (verify::same(s.value(), "----- The following text is needed only for the parser -----", false, true, false)) {
        begin_reading = true;
    }
    return std::nullopt;
}
