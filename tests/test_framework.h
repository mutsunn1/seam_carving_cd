#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace seam_carving {
namespace test {

// 轻量测试框架：自动收集 TEST 宏注册的用例，运行后输出 PASS/FAIL 统计
struct TestRunner {
    struct TestCase {
        std::string name;
        void (*func)();
    };

    static std::vector<TestCase>& tests() {
        static std::vector<TestCase> instance;
        return instance;
    }

    static void register_test(const std::string& name, void (*func)()) {
        tests().push_back({name, func});
    }

    static int run_all() {
        return run_with_prefix("");
    }

    // 只运行名字以 prefix 开头（prefix 为空则运行全部）的测试用例
    static int run_with_prefix(const std::string& prefix) {
        int passed = 0;
        int failed = 0;
        int skipped = 0;
        for (const auto& t : tests()) {
            if (!prefix.empty() && t.name.rfind(prefix, 0) != 0) {
                ++skipped;
                continue;
            }
            try {
                t.func();
                std::cout << "[PASS] " << t.name << "\n";
                ++passed;
            } catch (const std::exception& e) {
                std::cout << "[FAIL] " << t.name << ": " << e.what() << "\n";
                ++failed;
            }
        }
        std::cout << "\n" << passed << " passed, " << failed << " failed";
        if (skipped > 0) {
            std::cout << ", " << skipped << " skipped by prefix '" << prefix << "'";
        }
        std::cout << "\n";
        return failed == 0 ? 0 : 1;
    }
};

} // namespace test
} // namespace seam_carving

// 注册一个测试用例
#define TEST(name)                                              \
    static void test_##name();                                  \
    static struct test_##name##_registrar {                     \
        test_##name##_registrar() {                             \
            seam_carving::test::TestRunner::register_test(      \
                #name, test_##name);                            \
        }                                                       \
    } test_##name##_instance;                                   \
    static void test_##name()

// 断言失败时抛出异常
#define FAIL(msg)                                               \
    do {                                                        \
        std::ostringstream oss;                                 \
        oss << msg << " at " << __FILE__ << ":" << __LINE__;   \
        throw std::runtime_error(oss.str());                    \
    } while (0)

// 断言条件为真
#define ASSERT_TRUE(cond)                                       \
    do {                                                        \
        if (!(cond)) {                                          \
            FAIL("Assertion failed: " #cond);                   \
        }                                                       \
    } while (0)

// 断言两值相等
#define ASSERT_EQ(a, b)                                         \
    do {                                                        \
        if ((a) != (b)) {                                       \
            std::ostringstream oss;                             \
            oss << #a " != " #b ": " << (a) << " vs " << (b);  \
            FAIL(oss.str());                                    \
        }                                                       \
    } while (0)
