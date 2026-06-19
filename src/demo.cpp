#include "seam_carving.h"

#include <filesystem>

namespace seam_carving {

// TODO: 第四位同学实现
// 执行一次 seam carving 演示流程：
//   1. 保存原图
//   2. 计算能量图
//   3. 找到第一条 seam，并在原图上用红色标出
//   4. 缩放到目标宽度
DemoResult run_demo(const cv::Mat& image, int target_width) {
    DemoResult result;
    result.original = image.clone();
    result.energy = cv::Mat::zeros(image.rows, image.cols, CV_64F);
    result.seam_overlay = image.clone();

    // 占位：调用 resize 使结果图达到目标宽度
    if (target_width < image.cols) {
        result.resized = shrink_width(image, target_width);
    } else {
        result.resized = expand_width(image, target_width);
    }

    return result;
}

// TODO: 第四位同学实现
// 保存演示结果到 output_dir：
//   original.png / energy.png / seam_overlay.png / resized.png / index.html
void save_demo_outputs(const DemoResult& result, const std::string& output_dir) {
    // 占位：仅创建输出目录
    std::filesystem::create_directories(output_dir);
}

} // namespace seam_carving
