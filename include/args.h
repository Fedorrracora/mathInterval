#ifndef ARGS_H
#define ARGS_H
#include <functional>
#include <string>
#include <iostream>
#include <set>

namespace args {
    static std::set<std::string> names1, names2;

    struct inner_type {
        std::string ch;
        std::string name;
        std::function<void(const std::vector<std::string> &)> func;
        std::size_t nargs;
        std::string docx;

        inner_type(std::string ch,
                   std::string name,
                   const std::size_t nargs,
                   std::string docx,
                   std::function<void(const std::vector<std::string> &)> func) :
            ch(std::move(ch)), name(std::move(name)), func(std::move(func)), nargs(nargs), docx(std::move(docx)) {
            if (this->ch != "-" && names1.contains(this->ch)) {
                std::cerr << "-" << this->ch << " already defined" << std::endl;
                exit(1);
            }
            if (this->name != "-" && names2.contains(this->name)) {
                std::cerr << "--" << this->name << " already defined" << std::endl;
                exit(1);
            }
            names1.insert(this->ch);
            names2.insert(this->name);
        }
    };

    static std::vector<inner_type> data;
    static std::vector<bool> used;

    inline void print_help() {
        for (auto &i : data) { std::cout << i.ch << '/' << i.name << ": " << i.docx << std::endl; }
        exit(EXIT_SUCCESS);
    }

    inline void init(const int argc, const char *argv[], const bool help_op = true, const bool empty_print = false) {
        std::vector<std::string> args(argv, argv + argc);
        if (empty_print) print_help();
        if (help_op) {
            data.emplace_back("", "help", 0, "print help",
                [](const std::vector<std::string> &) { print_help(); });
        }
    }
}

#define ARGS_CONCAT_(a, b) a##b

#define ARGS_IN(ch, name, nargs, docx, priority)                                \
namespace args {                                                        \
    void ARGS_CONCAT_(func_,                                  \
    priority)(const std::vector<std::string> &);                 \
    static bool ARGS_CONCAT_(dummy_, priority) = []() {            \
    data.emplace_back((ch), (name), (nargs), (docx),                                             \
    ARGS_CONCAT_(func_, priority));      \
    return true;                                                               \
}();                                                                         \
}                                                                            \
void args::ARGS_CONCAT_(func_,                       \
priority)(const std::vector<std::string> &string)

#define ADD_ARG(ch, name, nargs, docx) ARGS_IN((#ch), (#name), (nargs), (docx), __COUNTER__)
#endif //ARGS_H
