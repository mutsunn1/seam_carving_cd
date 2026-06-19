#include "seam_carving.h"

namespace seam_carving {

// TODO: 第三位同学实现
// 删除一条竖向 seam，使图像宽度减 1：
//   对每一行，把 seam 指定列左右两侧的像素拼接起来
cv::Mat remove_vertical_seam(const cv::Mat& image, const std::vector<int>& seam) {
    // 占位：直接裁剪掉最右边一列
    return image(cv::Range::all(), cv::Range(0, image.cols - 1)).clone();
}

} // namespace seam_carving
