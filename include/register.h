#ifndef MATHINTERVAL_REGISTER_H
#define MATHINTERVAL_REGISTER_H
#include <utility>
#include <vector>
#include <string>
#include <functional>
#include <optional>
namespace formatter {
    struct test_t {
        std::string name;
        std::function<std::optional<std::string>(const std::optional<std::string> &)> func;
        test_t(std::string name, std::function<std::optional<std::string>(const std::optional<std::string> &)> func) : name(std::move(name)), func(std::move(func)) {}
    };
    inline std::vector<test_t> tests;
}
#define REGISTER_CONCAT_(a, b) a##b
#define REGISTER_UNIQUE_NAME_(prefix) REGISTER_CONCAT_(prefix, __COUNTER__)
/**
 * Register test.
 * Input - std::optional<std::string>. Returning value also std::optional<std::string>
 * If input - std::nullopt, it is last call of this function.
*/
#define REGISTER(test_name, input) \
namespace formatter { \
std::optional<std::string> REGISTER_UNIQUE_NAME_(formatter_test_func_) (const std::optional<std::string>& input); \
static bool REGISTER_UNIQUE_NAME_(formatter_dummy_) = []() { \
tests.emplace_back(#test_name, REGISTER_UNIQUE_NAME_(formatter_test_func_)); \
return true; \
}(); \
} \
std::optional<std::string> formatter::REGISTER_UNIQUE_NAME_(formatter_test_func_) (const std::optional<std::string>& input)
#endif //MATHINTERVAL_REGISTER_H