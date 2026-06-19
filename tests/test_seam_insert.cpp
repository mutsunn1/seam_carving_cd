#include "test_framework.h"
#include "seam_carving.h"

// 契约测试：插入 seam 后高度不变、宽度加 1、类型保持一致
TEST(seam_insert_increases_width_by_one) {
    cv::Mat image = cv::Mat::ones(4, 6, CV_8UC3);
    std::vector<int> seam = {1, 2, 1, 2};

    cv::Mat expanded = seam_carving::insert_vertical_seam(image, seam);

    ASSERT_EQ(expanded.rows, 4);
    ASSERT_EQ(expanded.cols, 7);
    ASSERT_EQ(expanded.type(), image.type());
}
