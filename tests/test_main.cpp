#include "test_framework.h"

// 测试入口：运行所有 TEST 宏注册的用例
int main() {
    return seam_carving::test::TestRunner::run_all();
}
