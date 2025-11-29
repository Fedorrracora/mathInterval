#include <bits/stdc++.h>
#include <interval.h>

int main() {
    interval::interval<std::string, interval::policy::int_type_policy> a;
    a.add_interval("1", "1000");
    a.remove_point("10");
    std::cout << a.print() << '\n';
    a += "0";
    std::cout << a.print() << '\n';
}
