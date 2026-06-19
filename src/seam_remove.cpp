#include "seam_carving.h"

namespace seam_carving {

// 删除一条竖向 seam（占位实现）
// 真实实现应：对每一行，去掉 seam 所在列后左右拼接
// 占位版本直接裁剪掉最右侧一列，满足宽度减 1、行数和类型不变等契约测试
cv::Mat remove_vertical_seam(const cv::Mat& image, const std::vector<int>& seam) {
    int new_w = std::max(1, image.cols - 1);
    cv::Mat result(image.rows, new_w, image.type());
    image(cv::Rect(0, 0, new_w, image.rows)).copyTo(result);
    return result;
}

} // namespace seam_carving
