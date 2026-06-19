"""生成演示用的示例图片。

本脚本仅用于准备数据文件，C++ 子项目本身不依赖 Python 运行时。
运行后会得到 data/sample.jpg，演示时可用：
    seam_carving_demo.exe data/sample.jpg output/resized.png --width 120
"""

import cv2
import numpy as np

height = 150
width = 200
image = np.zeros((height, width, 3), dtype=np.uint8)

# 左侧红色竖条（高频边缘）
image[:, :40] = [0, 0, 255]

# 右侧绿色竖条（高频边缘）
image[:, 160:] = [0, 255, 0]

# 中间蓝色平滑渐变区域，seam carving 缩图时会优先从这里删除 seam
for x in range(40, 160):
    image[:, x] = [255, 128, int(128 + 127 * (x - 40) / 119)]

# 添加几条水平纹理，让缩图效果更明显
for y in range(20, height, 40):
    image[y:y+2, 40:160] = [255, 255, 255]

cv2.imwrite("data/sample.jpg", image)
print("已生成 data/sample.jpg")
