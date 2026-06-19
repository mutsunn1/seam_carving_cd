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

        // 2. 运行演示流程：能量图、seam、缩放
        auto result = seam_carving::run_demo(image, target_width);

        // 3. 保存结果图片和 HTML 页面
        seam_carving::save_demo_outputs(result, output_dir);

        std::cout << "完成，输出保存在：" << output_dir << "\n";
    } catch (const std::exception& e) {
        std::cerr << "错误：" << e.what() << "\n";
        return 1;
    }

    return 0;
}
