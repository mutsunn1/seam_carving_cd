#include "test_framework.h"

#include <string>

// 测试入口：
//   无参数时运行所有用例；
//   有参数时运行名字以该参数为前缀的用例，便于 CI 按模块定位失败。
int main(int argc, char* argv[]) {
    if (argc > 1) {
        return seam_carving::test::TestRunner::run_with_prefix(argv[1]);
    }
    return seam_carving::test::TestRunner::run_all();
}
