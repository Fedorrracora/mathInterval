#include <bits/stdc++.h>
#include <interval.h>

int main() {
    interval::interval<int> a;
    std::cout << a.print() << '\n';
    a.apply_policy(interval::policy::empty_print_policy("ads"));
    std::cout << a.print() << '\n';
    a.add_interval(1, 2);
    std::cout << a.print() << '\n';
    a.add_interval(interval::minimal<int>(), -100);
    std::cout << a.print() << '\n';
    a.apply_policy(interval::policy::minmax_print_policy("-", "+"));
    std::cout << a.print() << '\n';
}
