#include "seam_carving.h"

namespace seam_carving {

// TODO: 第二位同学实现
// 用动态规划搜索竖向最小能量 seam：
//   1. dp[i][j] 表示从第一行走到 (i,j) 的最小累计能量
//   2. 初始化：dp[0][j] = energy[0][j]
//   3. 状态转移：dp[i][j] = energy[i][j] + min(dp[i-1][j-1], dp[i-1][j], dp[i-1][j+1])
//   4. 在最后一行找到最小值，向上回溯得到 seam
std::vector<int> find_vertical_seam(const cv::Mat& energy) {
    // 占位：返回每行中间列
    return std::vector<int>(energy.rows, energy.cols / 2);
}

} // namespace seam_carving
