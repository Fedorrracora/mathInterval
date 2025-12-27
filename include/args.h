#ifndef ARGS_H
#define ARGS_H
#include <functional>
#include <iostream>
#include <set>
#include <string>

namespace args {
    struct inner_type {
        std::string ch;
        std::string name;
        std::function<void(const std::vector<std::string> &)> func;
        std::size_t nargs;
        std::string docx;

        inner_type(std::string ch, std::string name, std::size_t nargs, std::string docx,
                   std::function<void(const std::vector<std::string> &)> func);
    };

    void print_args();

    std::size_t init(int argc, const char *argv[], bool skip_unknown = false);

    namespace detail {
        inline std::set<std::string> names1, names2;
        inline std::vector<inner_type> data;
        inline std::vector<bool> used;
    } // namespace detail
} // namespace args

#define ARGS_CONCAT_(a, b) a##b

#define ARGS_IN(ch, name, nargs, docx, priority)                                                                       \
    namespace args {                                                                                                   \
        void ARGS_CONCAT_(func_, priority)(const std::vector<std::string> &);                                          \
        static bool ARGS_CONCAT_(dummy_, priority) = []() {                                                            \
            detail::used.emplace_back(),                                                                               \
                detail::data.emplace_back((ch), (name), (nargs), (docx), ARGS_CONCAT_(func_, priority));               \
            return true;                                                                                               \
        }();                                                                                                           \
    }                                                                                                                  \
    void args::ARGS_CONCAT_(func_, priority)(const std::vector<std::string> &input)

#define ADD_ARG(ch, name, nargs, docx) ARGS_IN((#ch), (#name), (nargs), (docx), __COUNTER__)
#endif // ARGS_H
