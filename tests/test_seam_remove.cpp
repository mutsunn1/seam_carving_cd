#include "test_framework.h"
#include "seam_carving.h"

// 契约测试：删除 seam 后高度不变、宽度减 1、类型保持一致
TEST(seam_remove_reduces_width_by_one) {
    cv::Mat image = cv::Mat::ones(4, 6, CV_8UC3);
    std::vector<int> seam = {1, 2, 1, 2};

    cv::Mat carved = seam_carving::remove_vertical_seam(image, seam);

    ASSERT_EQ(carved.rows, 4);
    ASSERT_EQ(carved.cols, 5);
    ASSERT_EQ(carved.type(), image.type());
}
