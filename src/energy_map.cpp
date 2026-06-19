#include "seam_carving.h"

namespace seam_carving {

// TODO: 第一位同学实现
// 计算能量图：
//   1. 将彩色图转为灰度图
//   2. 用 Sobel 算子分别计算 x、y 方向梯度
//   3. 用梯度幅值作为能量，返回 CV_64F 单通道图
cv::Mat compute_energy_map(const cv::Mat& image) {
    // 占位：返回与输入同尺寸的全零图
    return cv::Mat::zeros(image.rows, image.cols, CV_64F);
}

} // namespace seam_carving
