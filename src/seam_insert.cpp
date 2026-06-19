#include "seam_carving.h"

namespace seam_carving {

// TODO: 第四位同学实现
// 插入一条竖向 seam，使图像宽度加 1：
//   对每一行，在 seam 右侧插入一个新像素
//   新像素值取 seam 位置与右侧相邻像素的平均值
cv::Mat insert_vertical_seam(const cv::Mat& image, const std::vector<int>& seam) {
    // 占位：返回一个宽度加 1 的图像（用全黑填充，只保证尺寸契约）
    return cv::Mat(image.rows, image.cols + 1, image.type(), cv::Scalar(0));
}

} // namespace seam_carving
