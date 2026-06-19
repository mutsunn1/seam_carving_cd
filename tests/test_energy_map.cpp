#include "test_framework.h"
#include "seam_carving.h"

#include <opencv2/core.hpp>

// 契约测试：能量图尺寸、类型正确，且能量值非负
TEST(energy_map_has_correct_size_and_type) {
    cv::Mat image = cv::Mat::ones(4, 6, CV_8UC3);
    cv::Mat energy = seam_carving::compute_energy_map(image);

    ASSERT_EQ(energy.rows, 4);
    ASSERT_EQ(energy.cols, 6);
    ASSERT_EQ(energy.channels(), 1);
    ASSERT_EQ(energy.depth(), CV_64F);
}

TEST(energy_map_values_are_non_negative) {
    cv::Mat image = cv::Mat::ones(4, 6, CV_8UC3);
    cv::Mat energy = seam_carving::compute_energy_map(image);

    for (int i = 0; i < energy.rows; ++i) {
        for (int j = 0; j < energy.cols; ++j) {
            ASSERT_TRUE(energy.at<double>(i, j) >= 0.0);
        }
    }
}
