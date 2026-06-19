#include "seam_carving.h"

#include <opencv2/imgproc.hpp>
#include <filesystem>
#include <fstream>

namespace seam_carving {

// 执行一次 seam carving 演示流程：
//   1. 计算能量图
//   2. 找到第一条最小能量 seam，并在原图上标红
//   3. 按目标宽度缩图或扩图
//   4. 返回原图、能量图、seam 覆盖图、结果图
DemoResult run_demo(const cv::Mat& image, int target_width) {
    DemoResult result;
    result.original = image.clone();

    // 计算能量图
    result.energy = compute_energy_map(image);

    // 找第一条 seam
    std::vector<int> seam = find_vertical_seam(result.energy);

    // 在原图上用红色标记 seam
    result.seam_overlay = image.clone();
    for (int i = 0; i < image.rows; ++i) {
        cv::Vec3b& p = result.seam_overlay.at<cv::Vec3b>(i, seam[i]);
        p[0] = 0;
        p[1] = 0;
        p[2] = 255;
    }

    // 缩放到目标宽度
    if (target_width < image.cols) {
        result.resized = shrink_width(image, target_width);
    } else {
        result.resized = expand_width(image, target_width);
    }

    return result;
}

// 保存演示结果到 output_dir：
//   original.png / energy.png / seam_overlay.png / resized.png / index.html
void save_demo_outputs(const DemoResult& result, const std::string& output_dir) {
    namespace fs = std::filesystem;
    fs::create_directories(output_dir);

    // 能量图归一化到 0-255，便于显示
    cv::Mat energy_vis;
    cv::normalize(result.energy, energy_vis, 0, 255, cv::NORM_MINMAX, CV_8U);

    // 保存四张图片
    save_image((fs::path(output_dir) / "original.png").string(), result.original);
    save_image((fs::path(output_dir) / "energy.png").string(), energy_vis);
    save_image((fs::path(output_dir) / "seam_overlay.png").string(), result.seam_overlay);
    save_image((fs::path(output_dir) / "resized.png").string(), result.resized);

    // 生成简单 HTML 页面
    std::ofstream html(fs::path(output_dir) / "index.html");
    html << "<!DOCTYPE html>\n"
         << "<html lang=\"zh-CN\">\n"
         << "<head><meta charset=\"UTF-8\"><title>Seam Carving</title></head>\n"
         << "<body style=\"text-align:center; font-family:sans-serif;\">\n"
         << "<h1>Seam Carving 演示</h1>\n"
         << "<p>原图：" << result.original.cols << " x " << result.original.rows
         << " &nbsp;→&nbsp; 结果：" << result.resized.cols << " x " << result.resized.rows << "</p>\n"
         << "<h3>原图</h3><img src=\"original.png\" style=\"max-width:400px;\"><br>\n"
         << "<h3>能量图</h3><img src=\"energy.png\" style=\"max-width:400px;\"><br>\n"
         << "<h3>Seam 覆盖图</h3><img src=\"seam_overlay.png\" style=\"max-width:400px;\"><br>\n"
         << "<h3>缩放结果</h3><img src=\"resized.png\" style=\"max-width:400px;\"><br>\n"
         << "</body>\n"
         << "</html>\n";
}

} // namespace seam_carving
