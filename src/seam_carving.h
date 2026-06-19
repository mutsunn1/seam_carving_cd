#pragma once

// Seam Carving 算法接口声明
// 所有模块的函数签名集中在这里，实现分散在 src/*.cpp 中

#include <opencv2/core.hpp>
#include <string>
#include <vector>

namespace seam_carving {

// 图片读写
cv::Mat load_image(const std::string& path);
void save_image(const std::string& path, const cv::Mat& image);

// 能量图：Sobel 梯度幅值
cv::Mat compute_energy_map(const cv::Mat& image);

// 动态规划找竖向最小能量 seam
std::vector<int> find_vertical_seam(const cv::Mat& energy);

// 删除 / 插入竖向 seam
cv::Mat remove_vertical_seam(const cv::Mat& image, const std::vector<int>& seam);
cv::Mat insert_vertical_seam(const cv::Mat& image, const std::vector<int>& seam);

// 缩图 / 扩图
cv::Mat shrink_width(const cv::Mat& image, int target_width);
cv::Mat expand_width(const cv::Mat& image, int target_width);

// 演示结果
struct DemoResult {
    cv::Mat original;
    cv::Mat energy;
    cv::Mat seam_overlay;
    cv::Mat resized;
};

// 执行一次 seam carving 演示流程
DemoResult run_demo(const cv::Mat& image, int target_width);

// 保存演示结果到目录，并生成 index.html
void save_demo_outputs(const DemoResult& result, const std::string& output_dir);

} // namespace seam_carving
