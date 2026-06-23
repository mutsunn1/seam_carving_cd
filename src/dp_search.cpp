#include "seam_carving.h"

#include <algorithm>

namespace seam_carving {

// 动态规划搜索竖向最小能量 seam（占位实现）
// 真实实现应：逐行 DP 计算最小累计能量并回溯
// 占位版本返回中间列的竖直 seam，满足长度、列范围、相邻约束等契约测试

std::vector<int> find_vertical_seam(const cv::Mat& energy)
{
    int h = energy.rows;
    int w = energy.cols;

    // dp[i][j]：从图像顶部走到 (i,j) 的最小累计能量
    cv::Mat dp(h, w, CV_64F);
    // prev[i][j]：记录(i,j)路径来自上一行哪一列，用于回溯缝
    cv::Mat prev(h, w, CV_32S);

    // 初始化第一行，无上层路径，总能量等于自身能量
    for (int j = 0; j < w; j++)
    {
        dp.at<double>(0, j) = energy.at<double>(0, j);
    }

    // 动态规划：逐行向下计算最小路径代价
    for (int i = 1; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            // 候选三个来源：左上、正上、右上
            int bestCol = j;
            double minCost = dp.at<double>(i - 1, j);

            // 左上
            if (j > 0)
            {
                double costLeftUp = dp.at<double>(i - 1, j - 1);
                if (costLeftUp < minCost)
                {
                    minCost = costLeftUp;
                    bestCol = j - 1;
                }
            }
            // 右上
            if (j + 1 < w)
            {
                double costRightUp = dp.at<double>(i - 1, j + 1);
                if (costRightUp < minCost)
                {
                    minCost = costRightUp;
                    bestCol = j + 1;
                }
            }

            // 更新当前位置最小总能量，记录前驱
            dp.at<double>(i, j) = energy.at<double>(i, j) + minCost;
            prev.at<int>(i, j) = bestCol;
        }
    }

    // 在最后一行找到总能量最小的列，作为缝的终点
    int endCol = 0;
    for (int k = 1; k < w; k++)
    {
        if (dp.at<double>(h - 1, k) < dp.at<double>(h - 1, endCol))
        {
            endCol = k;
        }
    }

    // 从下往上回溯，生成完整缝
    std::vector<int> seam(h);
    int curCol = endCol;
    seam[h - 1] = curCol;

    for (int i = h - 1; i > 0; i--)
    {
        curCol = prev.at<int>(i, curCol);
        seam[i - 1] = curCol;
    }

    return seam;
}

} // namespace seam_carving
