#include <args.h>
#include <sstream>
namespace args {
    inner_type::inner_type(std::string ch, std::string name, const std::size_t nargs, std::string docx,
                           std::function<void(const std::vector<std::string> &)> func) :
        ch(std::move(ch)), name(std::move(name)), func(std::move(func)), nargs(nargs), docx(std::move(docx)) {
        if (this->ch.size() != 1) {
            std::cerr << "-" << this->ch << " cannot be control char" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (this->ch != "-" && detail::names1.contains(this->ch)) {
            std::cerr << "-" << this->ch << " already defined" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (this->name != "-" && detail::names2.contains(this->name)) {
            std::cerr << "--" << this->name << " already defined" << std::endl;
            exit(EXIT_FAILURE);
        }
        detail::names1.insert(this->ch);
        detail::names2.insert(this->name);
    }

    [[nodiscard]] std::string print_using(const inner_type &el) {
        std::stringstream out;
        if (el.ch != "-") out << "-" << el.ch;
        if (el.ch != "-" && el.name != "-") out << " / ";
        if (el.name != "-") out << "--" << el.name;
        if (el.ch == "-" && el.name == "-") out << "no usage";
        return out.str();
    }

    void print_args() {
        std::cout << "Available args: \n";
        for (auto &i : detail::data) {
            std::cout << print_using(i) << ": " << i.docx << std::endl;
        }
    }

    std::size_t init(const int argc, const char *argv[], const bool skip_unknown) {
        const std::vector<std::string> args(argv, argv + argc);

        int sub_index = 0;
        for (auto i = 1; i < argc;) {
            std::string x;
            int y = -1;
            bool no_skip = false;
            if (args[i].starts_with("--")) {
                if (sub_index) sub_index = 0, ++i;
                x = args[i].substr(2);
                for (auto j = 0; j < detail::data.size(); ++j) {
                    if (detail::data[j].name != "-" && detail::data[j].name == x) {
                        y = j;
                        if (detail::used[j]) {
                            std::cerr << "flag repeated: " << print_using(detail::data[j]) << std::endl;
                            exit(EXIT_FAILURE);
                        }

                        detail::used[j] = true;
                        break;
                    }
                }
            }
            else if (args[i].starts_with("-")) {
                no_skip = true;
                if (sub_index + 1 < args[i].size()) { x = args[i].substr(++sub_index, 1); }
                else {
                    ++i;
                    sub_index = 0;
                    continue;
                }
                for (auto j = 0; j < detail::data.size(); ++j) {
                    if (detail::data[j].ch != "-" && detail::data[j].ch == x) {
                        y = j;
                        if (detail::used[j]) {
                            std::cerr << "flag repeated: " << print_using(detail::data[j]) << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        detail::used[j] = true;
                        break;
                    }
                }
            }
            else {
                if (sub_index) ++i;
                return static_cast<std::size_t>(i);
            }
            if (y == -1) {
                std::cerr << "unknown flag: ";
                if (no_skip)
                    std::cerr << "-" << x << std::endl;
                else
                    std::cerr << args[i] << std::endl;

                if (skip_unknown) continue;
                exit(EXIT_FAILURE);
            }
            std::vector<std::string> vec;
            vec.reserve(detail::data[y].nargs);
            if (detail::data[y].nargs) {
                if (no_skip && sub_index + 1 != args[i].size()) {
                    std::cerr << "incorrect using flags" << std::endl;
                    exit(EXIT_FAILURE);
                }
                ++i, sub_index = 0;
            }
            else {
                if (!no_skip) ++i;
            }
            for (auto j = 0; j < detail::data[y].nargs; ++j) {
                if (i >= args.size()) {
                    std::cerr << "incorrect using flags (lacks arguments)" << std::endl;
                    exit(EXIT_FAILURE);
                }
                vec.push_back(args[i]);
                ++i;
            }
            detail::data[y].func(vec);
        }
        return argc;
    }
} // namespace args
