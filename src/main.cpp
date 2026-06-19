#include "seam_carving.h"

#include <iostream>
#include <stdexcept>
#include <string>

// 命令行入口
// 用法：seam_carving_demo input.jpg output_dir --width 300
int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "用法: " << argv[0] << " <输入图片> <输出目录> --width <目标宽度>\n";
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_dir = argv[2];
    int target_width = std::stoi(argv[4]);

    try {
        // 1. 读取输入图片
        cv::Mat image = seam_carving::load_image(input_path);

        // 2. 生成前端动画：同时生成缩图和扩图，并在 HTML 中提供切换按钮
        // 缩图目标为命令行参数；扩图目标保证与缩图对称，使两边动画都足够明显
        int shrink_target = std::min(target_width, image.cols);
        int expand_target = std::max(target_width, image.cols);
        if (shrink_target == image.cols) {
            shrink_target = std::max(1, image.cols - 50);
        }
        if (expand_target == image.cols) {
            expand_target = image.cols + 50;
        }
        auto shrink_animation = seam_carving::animate_shrink(image, shrink_target);
        auto expand_animation = seam_carving::animate_expand(image, expand_target);
        seam_carving::save_animations(shrink_animation, expand_animation, output_dir);

        std::cout << "完成，输出保存在：" << output_dir << "\n";
        std::cout << "动画演示：" << output_dir << "\\animation.html\n";
    } catch (const std::exception& e) {
        std::cerr << "错误：" << e.what() << "\n";
        return 1;
    }

    return 0;
}
