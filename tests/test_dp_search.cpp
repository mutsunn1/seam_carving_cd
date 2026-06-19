#include "test_framework.h"
#include "seam_carving.h"

#include <algorithm>

// 契约测试：seam 长度、列号范围、相邻行变化幅度均合法
TEST(dp_search_returns_seam_with_correct_length) {
    cv::Mat energy = cv::Mat::ones(5, 3, CV_64F);
    std::vector<int> seam = seam_carving::find_vertical_seam(energy);

    ASSERT_EQ(static_cast<int>(seam.size()), 5);
}

TEST(dp_search_returns_valid_columns) {
    cv::Mat energy = cv::Mat::ones(5, 3, CV_64F);
    std::vector<int> seam = seam_carving::find_vertical_seam(energy);

    for (int col : seam) {
        ASSERT_TRUE(col >= 0 && col < 3);
    }
}

TEST(dp_search_adjacent_columns_differ_by_at_most_one) {
    cv::Mat energy = cv::Mat::ones(5, 3, CV_64F);
    std::vector<int> seam = seam_carving::find_vertical_seam(energy);

    for (size_t i = 1; i < seam.size(); ++i) {
        ASSERT_TRUE(std::abs(seam[i] - seam[i - 1]) <= 1);
    }
}
