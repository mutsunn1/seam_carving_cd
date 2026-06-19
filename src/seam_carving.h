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

// 前端动画演示：把缩图/扩图过程拆成一帧帧
struct AnimationFrame {
    cv::Mat image;         // 当前步骤图像
    cv::Mat energy;        // 能量图可视化
    cv::Mat seam_overlay;  // 标红 seam 的图像
};
using Animation = std::vector<AnimationFrame>;

// 生成缩图/扩图过程的逐帧动画
Animation animate_shrink(const cv::Mat& image, int target_width);
Animation animate_expand(const cv::Mat& image, int target_width);

// 同时保存缩图和扩图动画，并生成带切换按钮的 animation.html
void save_animations(const Animation& shrink, const Animation& expand, const std::string& output_dir);

} // namespace seam_carving
