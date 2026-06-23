#include "seam_carving.h"
#include <algorithm>
#include <opencv2/core.hpp>

namespace seam_carving
{

    /**
     * @brief 删除一条竖向 seam，使图像宽度减 1
     * @param image 输入的图像（支持单通道灰度图或多通道彩色图，如 CV_8UC3）
     * @param seam 长度等于图像高度的列号序列，seam[i] 表示第 i 行要删除的列号
     * @return cv::Mat 高度不变、宽度减 1、类型相同的新图像矩阵
     */
    cv::Mat remove_vertical_seam(const cv::Mat &image, const std::vector<int> &seam)
    {
        // 1. 严格契约与鲁棒性防御检查
        CV_Assert(!image.empty());
        CV_Assert(seam.size() == static_cast<size_t>(image.rows));
        CV_Assert(image.cols > 1); // 图像宽度必须大于 1 才能执行裁剪逻辑

        int h = image.rows;
        int w = image.cols;
        int channels = image.channels();

        // 2. 创建高度相同、宽度减 1、类型完全保持一致的目标矩阵
        cv::Mat result(h, w - 1, image.type());

        // 3. 逐行执行像素无缝拼接
        for (int i = 0; i < h; ++i)
        {
            int k = seam[i]; // 当前行要裁剪删除的列索引

            // 强校验：确保当前行的 seam 列号不发生越界风险
            CV_Assert(k >= 0 && k < w);

            // 获取原图像和目标图像第 i 行的行首连续内存指针
            const uchar *src = image.ptr<uchar>(i);
            uchar *dst = result.ptr<uchar>(i);

            // 计算裁剪点左右两侧像素区域占用的总字节数
            int left_len = k * channels;            // seam 左侧需要保留的字节数
            int right_len = (w - k - 1) * channels; // seam 右侧需要保留的字节数

            // 第一步：若左侧有像素，完整拷贝 seam 左侧的所有像素字节块
            if (left_len > 0)
            {
                std::copy(src, src + left_len, dst);
            }

            // 第二步：若右侧有像素，跳过当前列对应的通道字节（src + left_len + channels），
            // 将右侧剩余所有像素连续拷贝到目标行对应的左侧拼接末尾处（dst + left_len）
            if (right_len > 0)
            {
                std::copy(src + left_len + channels, src + left_len + channels + right_len, dst + left_len);
            }
        }

        return result;
    }

} // namespace seam_carving