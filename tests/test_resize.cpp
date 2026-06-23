#include "test_framework.h"
#include "seam_carving.h"

// 契约测试：缩图 / 扩图后达到目标宽度，且高度、类型保持不变
TEST(resize_shrink_width_reaches_target) {
    cv::Mat image = cv::Mat::ones(10, 8, CV_8UC3);
    cv::Mat resized = seam_carving::shrink_width(image, 5);

    ASSERT_EQ(resized.cols, 5);
    ASSERT_EQ(resized.rows, 10);
    ASSERT_EQ(resized.type(), image.type());
}

TEST(resize_expand_width_reaches_target) {
    cv::Mat image = cv::Mat::ones(10, 5, CV_8UC3);
    cv::Mat resized = seam_carving::expand_width(image, 8);

    ASSERT_EQ(resized.cols, 8);
    ASSERT_EQ(resized.rows, 10);
    ASSERT_EQ(resized.type(), image.type());
}
