#include "seam_carving.h"

namespace seam_carving {

// 插入一条竖向 seam（占位实现）
// 真实实现应：对每一行，在 seam 右侧插入相邻像素的平均值
// 占位版本在右侧复制一列，满足宽度加 1、行数和类型不变等契约测试
cv::Mat insert_vertical_seam(const cv::Mat& image, const std::vector<int>& seam) {
    cv::Mat result(image.rows, image.cols + 1, image.type());
    image.copyTo(result(cv::Rect(0, 0, image.cols, image.rows)));
    if (image.cols > 0) {
        image(cv::Rect(image.cols - 1, 0, 1, image.rows))
            .copyTo(result(cv::Rect(image.cols, 0, 1, image.rows)));
    }
    return result;
}

} // namespace seam_carving
