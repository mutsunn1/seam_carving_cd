#include "seam_carving.h"

namespace seam_carving {

// 反复删除 seam，直到图像宽度等于 target_width
cv::Mat shrink_width(const cv::Mat& image, int target_width) {
    cv::Mat result = image.clone();
    while (result.cols > target_width) {
        // 每轮：算能量 -> 找 seam -> 删除 seam
        cv::Mat energy = compute_energy_map(result);
        std::vector<int> seam = find_vertical_seam(energy);
        result = remove_vertical_seam(result, seam);
    }
    return result;
}

// 反复插入 seam，直到图像宽度等于 target_width
cv::Mat expand_width(const cv::Mat& image, int target_width) {
    cv::Mat result = image.clone();
    while (result.cols < target_width) {
        // 每轮：算能量 -> 找 seam -> 插入 seam
        cv::Mat energy = compute_energy_map(result);
        std::vector<int> seam = find_vertical_seam(energy);
        result = insert_vertical_seam(result, seam);
    }
    return result;
}

} // namespace seam_carving
