#include <algorithm>
#include <test_base/copy_count_base.h>
#define interval_H
#include <additional_test_tools.h>

std::pair<std::vector<std::string>, bool> line(const copy_count::detail::inner_type &a) {
    const auto [without_counter, with_counter] = a.call();
    std::string status;
    if (without_counter == a.without_counter && with_counter == a.with_counter) {
        status = "OK";
    }
    else if (without_counter <= a.without_counter && with_counter <= a.with_counter) {
        status = "WARNING";
    }
    else status = "ERROR";
    std::vector out = {
        std::to_string(a.priority),
        a.name,
        std::to_string(without_counter) + " (" + std::to_string(a.without_counter) + ")",
        std::to_string(with_counter) + " (" + std::to_string(a.with_counter) + ")",
        status
    };
    return {out, status == "OK"};
}

TEST(COPY_COUNT, copy_count) {
    copy_count::detail::data.emplace_back("in (point)", []()->copy_count::interval_t {
        return {};
    }, 0, [](const copy_count::interval_t& a)->void {
        auto ans = a.in(4);
        verify::DoNotOptimize(ans);
    }, 0, [](const copy_count::interval_t& a)->void {
        const verify::copy_counter x = 4;
        auto ans = a.in(x);
        verify::DoNotOptimize(ans);
    });
    copy_count::detail::data.emplace_back("in_v (point)", []()->copy_count::interval_t {
        return {};
    }, 0, [](const copy_count::interval_t& a)->void {
        auto ans = a.in_v(copy_count::interval_t::inp_type(4));
        verify::DoNotOptimize(ans);
    }, 0, [](const copy_count::interval_t& a)->void {
        const copy_count::interval_t::inp_type x = 4;
        auto ans = a.in_v(x);
        verify::DoNotOptimize(ans);
    });
    copy_count::detail::data.emplace_back("in_v (point, castable)", []()->copy_count::interval_t {
        return {};
    }, 0, [](const copy_count::interval_t& a)->void {
        auto ans = a.in_v(4);
        verify::DoNotOptimize(ans);
    }, 1, [](const copy_count::interval_t& a)->void {
        const verify::copy_counter x = 4;
        auto ans = a.in_v(x);
        verify::DoNotOptimize(ans);
    });

    copy_count::detail::data.emplace_back("add_point (new element)", []()->copy_count::interval_t {
        return {};
    }, 0, [](copy_count::interval_t a)->void {
        a.add_point(4);
    }, 1, [](copy_count::interval_t a)->void {
        verify::copy_counter x = 4;
        a.add_point(x);
    });
    copy_count::detail::data.emplace_back("add_point (already exist)", []()->copy_count::interval_t {
        auto x = copy_count::interval_t();
        x.add_point(4);
        return x;
    }, 0, [](copy_count::interval_t a)->void {
        a.add_point(4);
    }, 0, [](copy_count::interval_t a)->void {
        verify::copy_counter x = 4;
        a.add_point(x);
    });

    std::vector<std::vector<std::string>> table = {
        {"line", "name", "copy counter (rvalue)", "copy counter (lvalue)", "status"},
        {}
    };
    bool all_correct = true;
    for (const auto &i : copy_count::detail::data) {
        auto [l, status] = line(i);
        table.push_back(std::move(l));
        all_correct &= status;
    }
    const auto table_str = verifier_tests::to_table(table);
    std::cout << table_str << std::endl;
    ASSERT_TRUE(all_correct);
    // interval::interval<int> x;
    // interval::interval<int>::inp_type y(3);
    // auto out = x.in_v(y);
}