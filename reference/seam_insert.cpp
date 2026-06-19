#include "seam_carving.h"

#include <algorithm>

namespace seam_carving {

// 插入一条竖向 seam，使图像宽度加 1
// 对每一行，在 seam 右侧插入一个新像素
// 新像素值取 seam 位置与右侧相邻像素的平均值
cv::Mat insert_vertical_seam(const cv::Mat& image, const std::vector<int>& seam) {
    int h = image.rows;
    int w = image.cols;
    int channels = image.channels();

    cv::Mat result(h, w + 1, image.type());

    for (int i = 0; i < h; ++i) {
        int k = seam[i];                              // 当前行 seam 所在列
        int right_k = std::min(k + 1, w - 1);         // 右侧相邻列（处理最右边界）
        const uchar* src = image.ptr<uchar>(i);
        uchar* dst = result.ptr<uchar>(i);

        int left_len = (k + 1) * channels;            // 左侧要拷贝的字节数
        std::copy(src, src + left_len, dst);

        // 插入新像素：左右平均值
        for (int c = 0; c < channels; ++c) {
            int a = src[k * channels + c];
            int b = src[right_k * channels + c];
            dst[left_len + c] = static_cast<uchar>((a + b) / 2);
        }

        // 拷贝 seam 右侧剩余像素
        int right_len = (w - k - 1) * channels;
        std::copy(src + left_len, src + left_len + right_len, dst + left_len + channels);
    }

    return result;
}

} // namespace seam_carving
