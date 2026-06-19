#include "test_framework.h"
#include "seam_carving.h"

// 契约测试：run_demo 返回的缩放图达到目标宽度
TEST(run_demo_resizes_to_target_width) {
    cv::Mat image = cv::Mat::ones(10, 8, CV_8UC3);
    auto result = seam_carving::run_demo(image, 5);

    ASSERT_EQ(result.resized.cols, 5);
    ASSERT_EQ(result.resized.rows, 10);
    ASSERT_EQ(result.original.cols, 8);
}
