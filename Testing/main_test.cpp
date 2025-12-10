#include <verifier.h>
#include <interval.h>
#include <bits/stdc++.h>
int main() {
    interval::interval<int> a;
    a.add_point(1);
    a.add_point(2);
    a.add_point(2);
    a.add_point(-5);
    a.add_point(0);
    a.add_point(0);
    std::cout << a.to_string();
    return 0;
}