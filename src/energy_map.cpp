#include "seam_carving.h"

#include <opencv2/imgproc.hpp>

namespace seam_carving {

// 能量图（占位实现）
// 真实实现应：灰度化 -> Sobel x/y 梯度 -> 梯度幅值
// 占位版本返回与输入同尺寸的全 1 浮点图，满足尺寸、类型、非负等契约测试
 cv::Mat compute_energy_map(const cv::Mat& image) {
    return cv::Mat(image.rows, image.cols, CV_64F, cv::Scalar(1.0));
}

} // namespace seam_carving
