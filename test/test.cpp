#include <iostream>
#include <optional>
#include "../include/interval.h"

int main() {
    interval::interval<int> a;
    std::cout << a.print() << "\n"; // *empty*
    a.add_point(5);
    std::cout << a.print() << "\n"; // {5}
    a.add_interval(1, 3);
    std::cout << a.print() << "\n"; // (1; 3) U {5}

    a.add_interval(interval::minimal<int>(), 2);
    std::cout << a.print() << "\n"; // (-INF; 3) U {5}

    interval::interval<int> b;
    b.add_interval(2, 10);

    auto c = a * b;
    std::cout << c.print() << "\n"; // (2; 3) U {5}
    std::cout << c.inverse().print() << "\n"; // (-INF; 2] U [3; 5) U (5; +INF)
    c.clear();
    c.add_interval(1, 4);
    // This is one implementation of the any() function for integers.
    // It will always return some point in the multitude if it exists.
    auto d = c.any(
            [](const auto &x)->std::optional<int> {return x - 1;},
            [](const auto &x)->std::optional<int> {return x + 1;},
            [](const auto &x, const auto &y)->std::optional<int>
            {
                if (x + 1 < y) {return x + 1;} return std::nullopt;
            },
            0);
    std::cout << d.value() << "\n"; // 2
    c.clear();
    c.add_interval(interval::minimal<int>(), 3);
    c.add_interval(90, interval::maximal<int>());
    c.add_interval(12, 50);
    c.add_interval(56, 70);
    c.add_point(50);
    c.add_point(71);
    c.add_point(90);
    c.add_point(9);
    std::cout << c.print() << "\n"; // (-INF; 3) U {9} U (12; 50] U (56; 70) U {71} U [90; +INF)
    // This function shifts all points of the multitude by 4 to the right,
    // limiting the range of values to 0 - 100
    // Essentially the same as
    // c += 4
    // auto x = interval::interval<int>();
    // x.add_interval(0, 100)
    // c &= x
    c = c.custom_transfer(
        [](const auto &x) {return x + 4;},
        0,
        100
        );
    std::cout << c.print() << "\n"; // (0; 7) U {13} U (16; 54] U (60; 74) U {75} U [94; 100)
}
