#include "seam_carving.h"

namespace seam_carving {

// 动态规划搜索竖向最小能量 seam
std::vector<int> find_vertical_seam(const cv::Mat& energy) {
    int h = energy.rows;
    int w = energy.cols;

    // dp[i][j]：从第一行走到 (i,j) 的最小累计能量
    cv::Mat dp(h, w, CV_64F);
    // prev[i][j]：(i,j) 的前驱列号，用于回溯
    cv::Mat prev(h, w, CV_32S);

    // 初始化第一行
    for (int j = 0; j < w; ++j) {
        dp.at<double>(0, j) = energy.at<double>(0, j);
    }

    // 状态转移：从左中右三个前驱中选累计能量最小的
    for (int i = 1; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int best_j = j;
            double best_cost = dp.at<double>(i - 1, j);

            if (j > 0 && dp.at<double>(i - 1, j - 1) < best_cost) {
                best_j = j - 1;
                best_cost = dp.at<double>(i - 1, j - 1);
            }
            if (j + 1 < w && dp.at<double>(i - 1, j + 1) < best_cost) {
                best_j = j + 1;
            }

            dp.at<double>(i, j) = energy.at<double>(i, j) + dp.at<double>(i - 1, best_j);
            prev.at<int>(i, j) = best_j;
        }
    }

    // 在最后一行找到累计能量最小的终点
    std::vector<int> seam(h);
    int j = 0;
    for (int k = 1; k < w; ++k) {
        if (dp.at<double>(h - 1, k) < dp.at<double>(h - 1, j)) {
            j = k;
        }
    }
    seam[h - 1] = j;

    // 向上回溯得到整条 seam
    for (int i = h - 1; i > 0; --i) {
        j = prev.at<int>(i, j);
        seam[i - 1] = j;
    }

    return seam;
}

} // namespace seam_carving
