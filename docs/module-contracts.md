> 本模块契约适用于 C++ 子项目。

# 模块契约

所有接口声明在 `src/seam_carving.h` 中。`src/` 中当前是占位代码，完整实现见 `reference/`。

## 协作方式

- 每个同学只改自己负责的 `src/*.cpp` 文件。
- `reference/` 里的同名 `.cpp` 文件是可直接复制到 `src/` 的参考实现。
- 测试是**契约测试**：只验证接口与基础不变量，占位代码和参考实现都应通过。
- CI 直接构建 `src/` 中的当前代码，不自动复制 `reference/`。
- **CI 为每个模块创建独立 job（共 6 个并行 job）**，失败时 job 名称即对应模块，再由下表映射到人。

## 人员分工

| 模块 | 负责文件 | 同学 |
|------|----------|------|
| energy_map | `src/energy_map.cpp` | 第一位 |
| dp_search | `src/dp_search.cpp` | 第二位 |
| seam_remove | `src/seam_remove.cpp` | 第三位 |
| seam_insert | `src/seam_insert.cpp` | 第四位 |
| demo | `src/demo.cpp` | 第四位 |
| resize | `src/resize.cpp` | 已实现，不对应个人 |

## 通用约定

- 图像使用 OpenCV `cv::Mat` 表示。
- 彩色图为 3 通道 `CV_8UC3`。
- 能量图为单通道浮点 `CV_64FC1`。
- seam 是 `std::vector<int>`，长度等于图像高度，`seam[i]` 表示第 `i` 行 seam 所在列。
- 相邻两行 seam 列坐标差的绝对值不超过 1。

## 契约测试清单

| 模块 | 契约 | 占位代码应满足 |
|------|------|----------------|
| energy_map | 输出与输入同尺寸、单通道 CV_64F、能量非负 | 返回同尺寸全 1 图 |
| dp_search | seam 长度等于高度、列号合法、相邻差 ≤1 | 返回每行中间列 |
| seam_remove | 输出高度不变、宽度减 1、类型不变 | 裁剪最右一列 |
| seam_insert | 输出高度不变、宽度加 1、类型不变 | 右侧复制一列 |
| resize | 最终宽度达到目标值、高度和类型不变 | 依赖删除/插入 seam |
| demo | 动画帧数正确、seam 标记可见、HTML 切换按钮存在 | 占位能量/占位 seam |

## 测试命名约定

所有测试用例名统一为 `<模块>_<描述>`，例如：

- `energy_map_has_correct_size_and_type`
- `dp_search_returns_valid_columns`
- `resize_shrink_width_reaches_target`
- `demo_animate_shrink_frame_count`

CI 通过前缀过滤只运行对应模块的用例：

```cmd
build/Release/seam_carving_tests.exe energy_map
build/Release/seam_carving_tests.exe dp_search
build/Release/seam_carving_tests.exe seam_remove
build/Release/seam_carving_tests.exe seam_insert
build/Release/seam_carving_tests.exe resize
build/Release/seam_carving_tests.exe demo
```

## 第一位同学：能量图

文件：`src/energy_map.cpp`

```cpp
cv::Mat compute_energy_map(const cv::Mat& image);
```

- 输入：彩色图。
- 输出：与输入同尺寸的单通道能量图。
- 契约：尺寸对、类型对（CV_64FC1）、所有能量值 ≥ 0。
- 实现建议：转灰度 → Sobel x → Sobel y → `cv::magnitude`。
- 参考：`reference/energy_map.cpp`

## 第二位同学：DP 搜索 seam

文件：`src/dp_search.cpp`

```cpp
std::vector<int> find_vertical_seam(const cv::Mat& energy);
```

- 输入：能量图。
- 输出：长度为图像高度的列号序列。
- 契约：列号在 `[0, width)` 内，相邻行差值 ≤ 1。
- 实现建议：
  1. 初始化 `dp[0][j] = energy[0][j]`。
  2. 逐行填表，每个位置从左上、正上、右上选最小累计能量。
  3. 在最后一行找最小值，向上回溯得到 seam。
- 参考：`reference/dp_search.cpp`

## 第三位同学：删除 seam

文件：`src/seam_remove.cpp`

```cpp
cv::Mat remove_vertical_seam(const cv::Mat& image, const std::vector<int>& seam);
```

- 输入：彩色图和 seam。
- 输出：宽度减 1 的新图像。
- 契约：高度不变、宽度减 1、类型不变。
- 实现建议：对每一行，把 seam 列左右两侧像素拼接起来。
- 参考：`reference/seam_remove.cpp`

## 第四位同学：插入 seam 扩图 + 简单前端演示

文件：`src/seam_insert.cpp`、`src/demo.cpp`

### 插入 seam

```cpp
cv::Mat insert_vertical_seam(const cv::Mat& image, const std::vector<int>& seam);
```

- 输入：彩色图和 seam。
- 输出：宽度加 1 的新图像。
- 契约：高度不变、宽度加 1、类型不变。
- 实现建议：在 seam 右侧插入一个新像素，新像素值取 seam 位置与右侧相邻像素的平均值。
- 参考：`reference/seam_insert.cpp`

### 简单前端演示

接口：

```cpp
Animation animate_shrink(const cv::Mat& image, int target_width);
Animation animate_expand(const cv::Mat& image, int target_width);
void save_animations(const Animation& shrink, const Animation& expand, const std::string& output_dir);
```

- `animate_shrink` / `animate_expand`：生成缩图/扩图过程的逐帧动画。
- `save_animations`：保存动画帧到 `frames/shrink/` 和 `frames/expand/`，并生成带“缩图/扩图”切换按钮的 `animation.html`。
- 契约：动画帧数正确、seam 标记颜色可见（缩图红色、扩图绿色）、HTML 包含切换按钮。
- 参考：`reference/demo.cpp`

## 完整验证

```cmd
run_tests.bat                 :: 运行全部测试
run_tests.bat Release energy_map :: 只运行 energy_map 模块测试
run_demo.bat data\sample.jpg output --width 120
```
