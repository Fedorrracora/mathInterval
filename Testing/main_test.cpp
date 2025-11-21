#include "../include/verifier.h"
#include <bits/stdc++.h>
int main() {
    const std::string a = " abc a          ";
    std::cout << std::boolalpha << verify::same(a, verify::read_file("Testing/main_test.cpp"), false, false, true);
    return 0;
}