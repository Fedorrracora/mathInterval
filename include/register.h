#ifndef MATHINTERVAL_REGISTER_H
#define MATHINTERVAL_REGISTER_H
#include <functional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <verifier.h>
namespace formatter {
struct test_t {
  std::string name;
  std::function<void(const std::string &)> func;
  test_t(std::string name, std::function<void(const std::string &)> func)
      : name(std::move(name)), func(std::move(func)) {}
};
inline std::vector<test_t> tests;
/// The numbers written here are approximate. They are replaced every time they
/// are executed
inline std::vector<std::vector<std::string>> vec;
inline void vec_reset() {
  vec = {{"function"},
         {},
         {"time of processing ... elems (... iterations)"},
         {"time of processing ... elems (one iteration)"},
         {"time of processing ... elems (... iterations)"},
         {"time of processing ... elems (one iteration)"},
         {"asymptotics coefficient"}};
}
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

inline void setup_default(const std::string &s) {
  std::stringstream ss(s);
  std::string iters, a1, a2;
  ss >> iters >> a1 >> a2;
  vec[FIRST_TIME].front() =
      "time of processing " + a1 + " elems (" + iters + " iterations)";
  vec[FIRST_TIME_ITERATION].front() =
      "time of processing " + a1 + " elems (one iteration)";
  vec[SECOND_TIME].front() =
      "time of processing " + a2 + " elems (" + iters + " iterations)";
  vec[SECOND_TIME_ITERATION].front() =
      "time of processing " + a2 + " elems (one iteration)";
}

inline void setup(const std::string &s, bool &conf, bool &begin_reading) {
  if (begin_reading) {
    if (!conf) {
      conf = true;
      setup_default(s);
    } else {
      const std::vector<std::string> info = verify::split(s);
      vec[FUNCTION].push_back(info[LINE_FUNCTION]);
      vec[FIRST_TIME].push_back(info[LINE_FIRST_TIME]);
      vec[FIRST_TIME_ITERATION].push_back(info[LINE_FIRST_TIME_ITERATION]);
      vec[SECOND_TIME].push_back(info[LINE_SECOND_TIME]);
      vec[SECOND_TIME_ITERATION].push_back(info[LINE_SECOND_TIME_ITERATION]);
      vec[ASYMPTOTICS_COEFFICIENT].push_back(
          info[LINE_ASYMPTOTICS_COEFFICIENT]);
    }
  }
  if (verify::same(
          s, "----- The following text is needed only for the formatter -----",
          false, true, false)) {
    begin_reading = true;
  }
}
} // namespace formatter
#define REGISTER_CONCAT_(a, b) a##b
/**
 * Register test.
 */
#define REGISTER_IN(test_name, input, priority)                                \
  namespace formatter {                                                        \
  void REGISTER_CONCAT_(formatter_test_func_,                                  \
                        priority)(const std::string &(input));                 \
  static bool REGISTER_CONCAT_(formatter_dummy_, priority) = []() {            \
    tests.emplace_back(#test_name,                                             \
                       REGISTER_CONCAT_(formatter_test_func_, priority));      \
    return true;                                                               \
  }();                                                                         \
  }                                                                            \
  void formatter::REGISTER_CONCAT_(formatter_test_func_,                       \
                                   priority)(const std::string &(input))
#define REGISTER(test_name)                                                    \
  REGISTER_IN(test_name, s, __COUNTER__) {                                     \
    static bool conf = false, begin_reading = false;                           \
    formatter::setup(s, conf, begin_reading);                                  \
  }
#endif // MATHINTERVAL_REGISTER_H
