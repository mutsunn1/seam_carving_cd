#include "seam_carving.h"

#include <opencv2/imgcodecs.hpp>
#include <filesystem>
#include <stdexcept>

namespace seam_carving {

// 用 OpenCV 读取一张彩色图片
cv::Mat load_image(const std::string& path) {
    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    if (image.empty()) {
        throw std::runtime_error("无法读取图片: " + path);
    }
    return image;
}

// 用 OpenCV 保存图片，自动创建父目录
void save_image(const std::string& path, const cv::Mat& image) {
    std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    if (!cv::imwrite(path, image)) {
        throw std::runtime_error("无法保存图片: " + path);
    }
}

} // namespace seam_carving
