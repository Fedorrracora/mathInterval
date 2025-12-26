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
            if (this->ch.size() != 1) {
                std::cerr << "-" << this->ch << " cannot be control char" << std::endl;
                exit(EXIT_FAILURE);
            }
            if (this->ch != "-" && names1.contains(this->ch)) {
                std::cerr << "-" << this->ch << " already defined" << std::endl;
                exit(EXIT_FAILURE);
            }
            if (this->name != "-" && names2.contains(this->name)) {
                std::cerr << "--" << this->name << " already defined" << std::endl;
                exit(EXIT_FAILURE);
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

    inline std::size_t init(const int argc, const char *argv[], const bool skip_unknown = false, const bool help_op = true, const bool empty_print = false) {
        const std::vector<std::string> args(argv, argv + argc);
        if (empty_print) print_help();
        if (help_op) {
            data.emplace_back("-", "help", 0, "print help",
                [](const std::vector<std::string> &) { print_help(); });
        }
        int sub_index = 0;
        for (auto i = 1; i < argc; ) {
            std::string x;
            int y = -1;
            if (args[i].starts_with("--")) {
                sub_index = 0;
                x = args[i].substr(2);
                for (auto j = 0; j < data.size(); ++j) {
                    if (data[j].name != "-" && data[j].name == x) {
                        y = j;
                        if (used[j]) {
                            std::cerr << "flag repeated" << args[i] << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        used[j] = true;
                        break;
                    }
                }
            }
            else if (args[i].starts_with("-")) {
                if (sub_index + 1 < args[i].size()) {
                    x = args[i].substr(++sub_index, 1);
                }
                else {
                    ++i;
                    continue;
                }
                for (auto j = 0; j < data.size(); ++j) {
                    if (data[j].ch != "-" && data[j].ch == x) {
                        y = j;
                        if (used[j]) {
                            std::cerr << "flag repeated" << args[i] << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        used[j] = true;
                        break;
                    }
                }
            }
            else {
                return static_cast<std::size_t>(i);
            }
            if (y == -1) {
                std::cerr << "unknown flag " << args[i] << std::endl;
                if (skip_unknown) continue;
                exit(EXIT_FAILURE);
            }
            std::vector<std::string> vec;
            vec.reserve(data[y].nargs);
            ++i;
            for (auto j = 0; j < data[y].nargs; ++j) {
                vec.push_back(args[i]);
                ++i;
            }
            data[y].func(vec);
        }
        return argc;
    }
}

#define ARGS_CONCAT_(a, b) a##b

#define ARGS_IN(ch, name, nargs, docx, priority)                                \
namespace args {                                                        \
    void ARGS_CONCAT_(func_,                                  \
    priority)(const std::vector<std::string> &);                 \
    static bool ARGS_CONCAT_(dummy_, priority) = []() {            \
    used.emplace_back(), \
    data.emplace_back((ch), (name), (nargs), (docx),                                             \
    ARGS_CONCAT_(func_, priority));      \
    return true;                                                               \
}();                                                                         \
}                                                                            \
void args::ARGS_CONCAT_(func_,                       \
priority)(const std::vector<std::string> &string)

#define ADD_ARG(ch, name, nargs, docx) ARGS_IN((#ch), (#name), (nargs), (docx), __COUNTER__)
#endif //ARGS_H
