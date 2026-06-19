#include "seam_carving.h"

#include <opencv2/imgproc.hpp>

namespace seam_carving {

// 使用 OpenCV Sobel 算子计算能量图
// 能量越大的位置，像素越“重要”，越不应该被删除
cv::Mat compute_energy_map(const cv::Mat& image) {
    // 1. 彩色图转灰度图
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // 2. 分别计算 x、y 方向的 Sobel 梯度
    cv::Mat grad_x, grad_y;
    cv::Sobel(gray, grad_x, CV_64F, 1, 0, 3);
    cv::Sobel(gray, grad_y, CV_64F, 0, 1, 3);

    // 3. 梯度幅值作为能量
    cv::Mat energy;
    cv::magnitude(grad_x, grad_y, energy);
    return energy;
}

} // namespace seam_carving
