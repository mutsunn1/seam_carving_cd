# Seam Carving C++/MSVC 


## 目录结构

```text
seam_carving_cpp/
├── CMakeLists.txt          # CMake 构建配置
├── build_msvc.bat          # MSVC 一键构建
├── run_tests.bat           # 运行测试
├── run_demo.bat            # 运行演示
├── use_reference.bat       # 复制参考实现到 src/
├── src/
│   ├── seam_carving.h      # 所有接口声明（唯一头文件）
│   ├── image_io.cpp        # 图片读写（已实现）
│   ├── energy_map.cpp      # 能量图（占位，第一位实现）
│   ├── dp_search.cpp       # DP 找 seam（占位，第二位实现）
│   ├── seam_remove.cpp     # 删除 seam（占位，第三位实现）
│   ├── seam_insert.cpp     # 插入 seam（占位，第四位实现）
│   ├── resize.cpp          # 缩图 / 扩图（已实现）
│   ├── demo.cpp            # 演示输出（占位，第四位实现）
│   └── main.cpp            # 命令行入口（已实现）
├── reference/              # 完整参考实现 .cpp，可直接复制到 src/
├── tests/                  # 契约测试
├── data/                   # 示例图片
└── output/                 # 演示输出
```

## 协作方式

1. 所有接口都在 `src/seam_carving.h` 中声明，每个同学只需修改自己负责的 `.cpp` 文件。
2. `reference/` 里有完整可运行的参考实现，直接复制到 `src/` 同名文件即可看到正确效果。
3. 测试是**契约测试**：只检查尺寸、类型、范围、相邻约束等基础不变量。**占位代码和参考实现都应通过测试**，这样任何一位同学未开始实现时，CI 也能通过，方便单独验证自己的模块。
4. 想一次性用全部参考实现跑通：

   ```cmd
   use_reference.bat
   build_msvc.bat
   ```

## 前置要求

- Visual Studio 2022，勾选“使用 C++ 的桌面开发”
- OpenCV 4.x C++ 库
- CMake 3.20+

## OpenCV 配置

构建前设置环境变量 `OPENCV_DIR`，指向包含 `OpenCVConfig.cmake` 的目录：

```cmd
set OPENCV_DIR=C:\opencv\build\x64\vc16\lib
```

运行前把 OpenCV 的 `build\x64\vc16\bin` 加入 `PATH`。

## 构建

```cmd
build_msvc.bat
```

## 运行测试

```cmd
run_tests.bat
```

## 运行演示

```cmd
run_demo.bat data\sample.jpg output --width 120
```

会在 `output/` 目录生成前端动画：

- `animation.html`
- `frames/shrink/*.png`
- `frames/expand/*.png`

用浏览器打开 `output/animation.html` 可以播放/拖动查看 seam carving 每一步的变化。

动画说明：

- 页面顶部有 **“缩图 / 扩图”** 切换按钮，可同时观看两种操作。
- **缩图**：红色粗线标记当前帧即将删除的 seam，下一帧图像宽度减 1。
- **扩图**：绿色粗线标记当前帧刚插入的 seam，可以清楚看到图像宽度逐步增加。
- 无论命令行 `--width` 是缩图还是扩图，动画页面都会同时生成两边的过程。

## 模块分工

| 同学 | 模块 | 文件 |
|------|------|------|
| 第一位 | 能量图 | `src/energy_map.cpp` |
| 第二位 | DP 搜索 seam | `src/dp_search.cpp` |
| 第三位 | 删除 seam | `src/seam_remove.cpp` |
| 第四位 | 插入 seam 扩图 + 简单前端演示 | `src/seam_insert.cpp`、`src/demo.cpp` |

## 算法思路

- **能量图**：灰度 + Sobel x/y 梯度 + 梯度幅值。
- **DP 找 seam**：`dp[i][j]` 表示从第一行走到 `(i,j)` 的最小累计能量；每个位置从左上、正上、右上选一个最小前驱；最后从最后一行最小值回溯。
- **删除 seam**：每行去掉 seam 所在列，左右拼接。
- **插入 seam**：每行在 seam 右侧插入相邻像素的平均值。
- **缩图 / 扩图**：循环找 seam、删除或插入，直到达到目标宽度。

## CI

`.github/workflows/ci-cpp.yml` 直接构建当前 `src/` 中的代码，并运行契约测试。因此：

- 任何同学 push 自己负责的文件，CI 都能跑。
- 其他同学还没实现时，CI 也能通过（因为占位代码满足契约测试）。
- 想本地看真实效果，手动运行 `use_reference.bat`。

test
test


