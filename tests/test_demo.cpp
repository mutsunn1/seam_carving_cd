#include "test_framework.h"
#include "seam_carving.h"

#include <filesystem>
#include <fstream>
#include <sstream>

// 契约测试：演示入口使用的缩图函数能达到目标宽度
TEST(demo_uses_resize_to_target_width) {
    cv::Mat image = cv::Mat::ones(10, 8, CV_8UC3);
    cv::Mat resized = seam_carving::shrink_width(image, 5);

    ASSERT_EQ(resized.cols, 5);
    ASSERT_EQ(resized.rows, 10);
}

// 契约测试：缩图动画的帧数等于宽度变化量 + 1
TEST(demo_animate_shrink_frame_count) {
    cv::Mat image = cv::Mat::ones(6, 8, CV_8UC3);
    auto animation = seam_carving::animate_shrink(image, 5);
    ASSERT_EQ(animation.size(), static_cast<size_t>(8 - 5 + 1));
}

// 契约测试：扩图动画的帧数等于宽度变化量 + 1
TEST(demo_animate_expand_frame_count) {
    cv::Mat image = cv::Mat::ones(6, 5, CV_8UC3);
    auto animation = seam_carving::animate_expand(image, 8);
    ASSERT_EQ(animation.size(), static_cast<size_t>(8 - 5 + 1));
}

// 辅助：统计图像中某种颜色的像素数
static int count_color_pixels(const cv::Mat& image, const cv::Vec3b& color, int tolerance) {
    int count = 0;
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            cv::Vec3b p = image.at<cv::Vec3b>(i, j);
            if (std::abs(p[0] - color[0]) <= tolerance &&
                std::abs(p[1] - color[1]) <= tolerance &&
                std::abs(p[2] - color[2]) <= tolerance) {
                ++count;
            }
        }
    }
    return count;
}

// 契约测试：缩图动画中，除最后一帧外，seam_overlay 都包含红色标记
TEST(demo_animate_shrink_marks_seam_red) {
    cv::Mat image = cv::Mat::ones(6, 8, CV_8UC3);
    auto animation = seam_carving::animate_shrink(image, 5);
    ASSERT_TRUE(animation.size() >= 2);
    for (size_t i = 0; i + 1 < animation.size(); ++i) {
        int red_count = count_color_pixels(animation[i].seam_overlay, cv::Vec3b(0, 0, 255), 10);
        ASSERT_TRUE(red_count > 0);
    }
}

// 契约测试：扩图动画中，除第一帧外，seam_overlay 都包含绿色插入标记
TEST(demo_animate_expand_marks_inserted_seam_green) {
    cv::Mat image = cv::Mat::ones(6, 5, CV_8UC3);
    auto animation = seam_carving::animate_expand(image, 8);
    ASSERT_TRUE(animation.size() >= 2);
    for (size_t i = 1; i < animation.size(); ++i) {
        int green_count = count_color_pixels(animation[i].seam_overlay, cv::Vec3b(0, 255, 0), 10);
        ASSERT_TRUE(green_count > 0);
    }
}

// 契约测试：save_animations 同时生成缩图/扩图帧和带切换按钮的 HTML
TEST(demo_save_animations_creates_both_modes) {
    namespace fs = std::filesystem;
    cv::Mat image = cv::Mat::ones(6, 8, CV_8UC3);
    auto shrink = seam_carving::animate_shrink(image, 6);
    auto expand = seam_carving::animate_expand(image, 10);

    std::string out_dir = "test_output_animations";
    fs::remove_all(out_dir);
    seam_carving::save_animations(shrink, expand, out_dir);

    ASSERT_TRUE(fs::exists(out_dir + "/animation.html"));
    ASSERT_TRUE(fs::exists(out_dir + "/frames/shrink/img_0000.png"));
    ASSERT_TRUE(fs::exists(out_dir + "/frames/expand/img_0000.png"));

    std::ifstream html(out_dir + "/animation.html");
    std::stringstream buffer;
    buffer << html.rdbuf();
    html.close();
    std::string content = buffer.str();
    ASSERT_TRUE(content.find("缩图") != std::string::npos);
    ASSERT_TRUE(content.find("扩图") != std::string::npos);
    ASSERT_TRUE(content.find("btn-shrink") != std::string::npos);
    ASSERT_TRUE(content.find("btn-expand") != std::string::npos);

    fs::remove_all(out_dir);
}
