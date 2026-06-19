#include "seam_carving.h"

#include <opencv2/imgproc.hpp>
#include <filesystem>
#include <fstream>

namespace seam_carving {

namespace {

// 在 BGR 图像上用彩色粗线标记 seam，默认 3 像素宽，便于前端观看
cv::Mat mark_seam_colored(const cv::Mat& image, const std::vector<int>& seam,
                          const cv::Vec3b& color, int half_thickness = 1) {
    cv::Mat result = image.clone();
    for (int i = 0; i < result.rows; ++i) {
        for (int dt = -half_thickness; dt <= half_thickness; ++dt) {
            int j = seam[i] + dt;
            if (j >= 0 && j < result.cols) {
                result.at<cv::Vec3b>(i, j) = color;
            }
        }
    }
    return result;
}

// 缩图：把即将删除的 seam 标红
cv::Mat mark_seam_for_removal(const cv::Mat& image, const std::vector<int>& seam) {
    return mark_seam_colored(image, seam, cv::Vec3b(0, 0, 255), 1);
}

// 扩图：把刚插入的 seam 标绿（插入位置在 seam 右侧一列）
cv::Mat mark_inserted_seam(const cv::Mat& image, const std::vector<int>& seam) {
    cv::Mat result = image.clone();
    for (int i = 0; i < result.rows; ++i) {
        int inserted_j = seam[i] + 1;
        for (int dt = -1; dt <= 1; ++dt) {
            int j = inserted_j + dt;
            if (j >= 0 && j < result.cols) {
                result.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 255, 0);
            }
        }
    }
    return result;
}

// 把浮点能量图映射成彩色图，便于前端显示
cv::Mat visualize_energy_color(const cv::Mat& energy) {
    cv::Mat gray;
    cv::normalize(energy, gray, 0, 255, cv::NORM_MINMAX, CV_8U);
    cv::Mat color;
    cv::applyColorMap(gray, color, cv::COLORMAP_JET);
    return color;
}

// 占位能量图：全 1 矩阵
cv::Mat placeholder_energy(const cv::Mat& image) {
    return cv::Mat(image.rows, image.cols, CV_64F, cv::Scalar(1.0));
}

// 占位 seam：中间列的竖直 seam
std::vector<int> placeholder_seam(int rows, int cols) {
    int mid = std::max(0, cols / 2);
    return std::vector<int>(rows, mid);
}

// 把整数序号补零到 4 位，用于文件名
std::string frame_index_str(size_t i) {
    std::string s = std::to_string(i);
    return std::string(4 - s.length(), '0') + s;
}

} // anonymous namespace

// 缩图动画（占位实现）
// 真实实现应：每轮计算真实能量图和最小能量 seam，再删除 seam
// 占位版本使用中间列作为 seam，仅保证动画结构正确、红/绿标记可见
Animation animate_shrink(const cv::Mat& image, int target_width) {
    Animation animation;
    cv::Mat current = image.clone();
    while (current.cols > target_width) {
        cv::Mat energy = placeholder_energy(current);
        std::vector<int> seam = placeholder_seam(current.rows, current.cols);
        animation.push_back({current.clone(), visualize_energy_color(energy), mark_seam_for_removal(current, seam)});
        current = remove_vertical_seam(current, seam);
    }
    cv::Mat final_energy = placeholder_energy(current);
    animation.push_back({current.clone(), visualize_energy_color(final_energy), current.clone()});
    return animation;
}

// 扩图动画（占位实现）
// 真实实现应：每轮计算真实能量图和最小能量 seam，再插入 seam
// 占位版本使用中间列作为 seam，仅保证动画结构正确、红/绿标记可见
Animation animate_expand(const cv::Mat& image, int target_width) {
    Animation animation;
    cv::Mat current = image.clone();
    animation.push_back({current.clone(), visualize_energy_color(placeholder_energy(current)), current.clone()});
    while (current.cols < target_width) {
        cv::Mat energy = placeholder_energy(current);
        std::vector<int> seam = placeholder_seam(current.rows, current.cols);
        cv::Mat next = insert_vertical_seam(current, seam);
        cv::Mat next_energy = placeholder_energy(next);
        animation.push_back({next.clone(), visualize_energy_color(next_energy), mark_inserted_seam(next, seam)});
        current = next;
    }
    return animation;
}

// 保存单个动画的辅助函数，把帧写入指定子目录
static void save_animation_frames(const Animation& animation, const std::string& output_dir,
                                  const std::string& subdir) {
    namespace fs = std::filesystem;
    fs::path frames_dir = fs::path(output_dir) / subdir;
    fs::create_directories(frames_dir);

    for (size_t i = 0; i < animation.size(); ++i) {
        std::string idx = frame_index_str(i);
        save_image((frames_dir / ("img_" + idx + ".png")).string(), animation[i].image);
        save_image((frames_dir / ("energy_" + idx + ".png")).string(), animation[i].energy);
        save_image((frames_dir / ("seam_" + idx + ".png")).string(), animation[i].seam_overlay);
    }
}

// 同时保存缩图和扩图动画，并生成带切换按钮的 animation.html
void save_animations(const Animation& shrink, const Animation& expand, const std::string& output_dir) {
    namespace fs = std::filesystem;
    fs::create_directories(output_dir);

    save_animation_frames(shrink, output_dir, "frames/shrink");
    save_animation_frames(expand, output_dir, "frames/expand");

    std::ofstream html(fs::path(output_dir) / "animation.html");
    html << "<!DOCTYPE html>\n"
         << "<html lang=\"zh-CN\">\n"
         << "<head>\n"
         << "<meta charset=\"UTF-8\">\n"
         << "<title>Seam Carving 动画演示</title>\n"
         << "<style>\n"
         << "body { font-family: sans-serif; text-align: center; background: #f5f5f5; margin: 0; padding: 20px; }\n"
         << "h1 { color: #333; margin-bottom: 10px; }\n"
         << ".mode-btns { margin: 10px 0 20px; }\n"
         << ".mode-btns button { padding: 8px 24px; margin: 0 5px; font-size: 14px; cursor: pointer; border: 1px solid #999; border-radius: 4px; background: #fff; }\n"
         << ".mode-btns button.active { background: #4a90d9; color: #fff; border-color: #4a90d9; }\n"
         << ".mode-btns button:hover:not(.active) { background: #eee; }\n"
         << ".stage { display: flex; justify-content: center; gap: 20px; flex-wrap: wrap; margin: 20px 0; }\n"
         << ".stage > div { background: #fff; padding: 10px; border-radius: 8px; box-shadow: 0 2px 6px rgba(0,0,0,0.1); }\n"
         << ".stage img { max-width: 300px; max-height: 300px; display: block; margin-top: 8px; border: 1px solid #ddd; }\n"
         << ".controls { margin: 15px 0; }\n"
         << ".controls button { padding: 8px 24px; margin: 0 5px; font-size: 14px; cursor: pointer; border: 1px solid #999; border-radius: 4px; background: #fff; }\n"
         << ".controls button:hover { background: #eee; }\n"
         << "input[type=range] { width: 400px; vertical-align: middle; }\n"
         << ".info { color: #666; margin-top: 10px; }\n"
         << ".legend { margin: 10px 0; font-size: 14px; }\n"
         << ".legend span { display: inline-block; margin: 0 12px; }\n"
         << ".red-box { display: inline-block; width: 14px; height: 14px; background: red; vertical-align: middle; margin-right: 4px; }\n"
         << ".green-box { display: inline-block; width: 14px; height: 14px; background: green; vertical-align: middle; margin-right: 4px; }\n"
         << "</style>\n"
         << "</head>\n"
         << "<body>\n"
         << "<h1>Seam Carving 动画演示</h1>\n"
         << "<div class=\"mode-btns\">\n"
         << "  <button id=\"btn-shrink\" class=\"active\">缩图</button>\n"
         << "  <button id=\"btn-expand\">扩图</button>\n"
         << "</div>\n"
         << "<div class=\"legend\">\n"
         << "  <span><span class=\"red-box\"></span>红色 = 缩图时即将删除的 seam</span>\n"
         << "  <span><span class=\"green-box\"></span>绿色 = 扩图时刚插入的 seam</span>\n"
         << "</div>\n"
         << "<div class=\"stage\">\n"
         << "  <div><strong>原图过程</strong><img id=\"img\" src=\"\"></div>\n"
         << "  <div><strong>能量图</strong><img id=\"energy\" src=\"\"></div>\n"
         << "  <div><strong>Seam 标记</strong><img id=\"seam\" src=\"\"></div>\n"
         << "</div>\n"
         << "<div class=\"controls\">\n"
         << "  <button id=\"playBtn\">播放</button>\n"
         << "  <input type=\"range\" id=\"slider\" min=\"0\" max=\"" << (shrink.size() - 1) << "\" value=\"0\">\n"
         << "  <span id=\"counter\">1 / " << shrink.size() << "</span>\n"
         << "</div>\n"
         << "<div class=\"info\">选择上方“缩图/扩图”按钮切换动画，拖动滑块或点击播放查看每一步</div>\n"
         << "<script>\n"
         << "const modes = {\n"
         << "  shrink: { total: " << shrink.size() << ", prefix: 'frames/shrink/' },\n"
         << "  expand: { total: " << expand.size() << ", prefix: 'frames/expand/' }\n"
         << "};\n"
         << "let currentMode = 'shrink';\n"
         << "let current = 0, timer = null, playing = false;\n"
         << "function pad(n) { return ('0000' + n).slice(-4); }\n"
         << "function show(i) {\n"
         << "  const mode = modes[currentMode];\n"
         << "  document.getElementById('img').src = mode.prefix + 'img_' + pad(i) + '.png';\n"
         << "  document.getElementById('energy').src = mode.prefix + 'energy_' + pad(i) + '.png';\n"
         << "  document.getElementById('seam').src = mode.prefix + 'seam_' + pad(i) + '.png';\n"
         << "  document.getElementById('slider').value = i;\n"
         << "  document.getElementById('slider').max = mode.total - 1;\n"
         << "  document.getElementById('counter').textContent = (i + 1) + ' / ' + mode.total;\n"
         << "}\n"
         << "function switchMode(mode) {\n"
         << "  pause();\n"
         << "  currentMode = mode;\n"
         << "  current = 0;\n"
         << "  document.getElementById('btn-shrink').classList.toggle('active', mode === 'shrink');\n"
         << "  document.getElementById('btn-expand').classList.toggle('active', mode === 'expand');\n"
         << "  show(0);\n"
         << "}\n"
         << "document.getElementById('btn-shrink').addEventListener('click', () => switchMode('shrink'));\n"
         << "document.getElementById('btn-expand').addEventListener('click', () => switchMode('expand'));\n"
         << "document.getElementById('slider').addEventListener('input', e => {\n"
         << "  current = parseInt(e.target.value);\n"
         << "  show(current);\n"
         << "  if (playing) pause();\n"
         << "});\n"
         << "function play() {\n"
         << "  playing = true;\n"
         << "  document.getElementById('playBtn').textContent = '暂停';\n"
         << "  timer = setInterval(() => {\n"
         << "    const mode = modes[currentMode];\n"
         << "    current = (current + 1) % mode.total;\n"
         << "    show(current);\n"
         << "  }, 200);\n"
         << "}\n"
         << "function pause() {\n"
         << "  playing = false;\n"
         << "  document.getElementById('playBtn').textContent = '播放';\n"
         << "  clearInterval(timer);\n"
         << "}\n"
         << "document.getElementById('playBtn').addEventListener('click', () => {\n"
         << "  playing ? pause() : play();\n"
         << "});\n"
         << "show(0);\n"
         << "</script>\n"
         << "</body>\n"
         << "</html>\n";
    html.close();
}

} // namespace seam_carving
