#include <gtest/gtest.h>
#include <string>
#include <fbp/forward_container.h>
TEST(FORWARD_CONTAINER, all_tests) {
    const std::string using_s = "abc";
    fbp::forward_container cont(using_s);
    EXPECT_TRUE(cont.call() == using_s && cont < std::string("b"))
            << "error in step 1: lvalue with call\n";
    EXPECT_TRUE(std::move(cont).get() == using_s)
            << "error in step 2: get from lvalue\n";
    std::string s = using_s;
    cont = fbp::forward_container(std::move(s));
    s = "a";
    EXPECT_TRUE(cont.call() == using_s && cont < std::string("b"))
            << "error in step 3: rvalue with call\n";
    EXPECT_TRUE(std::move(cont).get() == using_s && s == "a")
            << "error in step 4: get from rvalue\n";
}