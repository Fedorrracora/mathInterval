#ifndef VERIFIER_H
#define VERIFIER_H
#include <string>
#include <sstream>
#include <chrono>
#include <random>

namespace verify {
    /// return string with content of file
    std::string read_file(const std::string &filename);

    /**
     return true if two strings are equal with some filters

     - case_sensitive - does what it should
     - whitespace_sensitive - repetitions of spaces, enter and special characters decodes such one space
     - boundary_sensitive - remove all spaces, enters and special characters from begin and end of the string
     */
    bool same(const std::string &a, const std::string &b, bool case_sensitive=false, bool whitespace_sensitive=false, bool boundary_sensitive=false);

    /// helps to read a long string by lines
    struct line_checker {
        /// last read line
        const std::string &last;
        explicit line_checker(std::string s);
        /// gets next line
        std::string operator()();
    private:
        std::stringstream contain;
        std::string last_str;
    };

    /// helps keep track of time
    struct time_checker {
        /// start timing
        void start();
        /// stop timing. Not work if you did not start
        void stop();
        /// get calculated time
        [[nodiscard]] double time() const;
    private:
        std::chrono::steady_clock::time_point begin_point, end_point;
        double len{};
        bool started = false;
    };

    inline std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

    /// return random value between from and to
    int random_int_distribution(int from, int to);
}
#endif //VERIFIER_H