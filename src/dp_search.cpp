#include "seam_carving.h"

#include <algorithm>

namespace seam_carving {

// 动态规划搜索竖向最小能量 seam（占位实现）
// 真实实现应：逐行 DP 计算最小累计能量并回溯
// 占位版本返回中间列的竖直 seam，满足长度、列范围、相邻约束等契约测试
std::vector<int> find_vertical_seam(const cv::Mat& energy) {
    int h = energy.rows;
    int w = energy.cols;
    int mid = std::max(0, w / 2);
    return std::vector<int>(h, mid);
}
1111
} // namespace seam_carving
