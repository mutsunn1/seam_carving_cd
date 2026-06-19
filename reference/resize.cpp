#include "seam_carving.h"

namespace seam_carving {

// 缩图：循环删除 seam 直到达到目标宽度
cv::Mat shrink_width(const cv::Mat& image, int target_width) {
    cv::Mat result = image.clone();
    while (result.cols > target_width) {
        cv::Mat energy = compute_energy_map(result);
        std::vector<int> seam = find_vertical_seam(energy);
        result = remove_vertical_seam(result, seam);
    }
    return result;
}

// 扩图：循环插入 seam 直到达到目标宽度
cv::Mat expand_width(const cv::Mat& image, int target_width) {
    cv::Mat result = image.clone();
    while (result.cols < target_width) {
        cv::Mat energy = compute_energy_map(result);
        std::vector<int> seam = find_vertical_seam(energy);
        result = insert_vertical_seam(result, seam);
    }
    return result;
}

} // namespace seam_carving
