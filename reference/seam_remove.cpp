#include "seam_carving.h"

namespace seam_carving {

// 删除一条竖向 seam，使图像宽度减 1
// 对每一行，把 seam 指定列左右两侧的像素拼接起来
cv::Mat remove_vertical_seam(const cv::Mat& image, const std::vector<int>& seam) {
    int h = image.rows;
    int w = image.cols;
    int channels = image.channels();

    cv::Mat result(h, w - 1, image.type());

    for (int i = 0; i < h; ++i) {
        int k = seam[i];                 // 当前行要删除的列
        const uchar* src = image.ptr<uchar>(i);
        uchar* dst = result.ptr<uchar>(i);

        int left_len = k * channels;     // seam 左侧像素占用的字节数
        int right_len = (w - k - 1) * channels;  // seam 右侧像素占用的字节数

        // 先拷贝左侧，再拷贝右侧（跳过 seam 列）
        std::copy(src, src + left_len, dst);
        std::copy(src + left_len + channels, src + left_len + channels + right_len, dst + left_len);
    }

    return result;
}

} // namespace seam_carving
