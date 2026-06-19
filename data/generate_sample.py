"""生成演示用的示例图片。

本脚本仅用于准备数据文件，C++ 子项目本身不依赖 Python 运行时。
运行后会得到 data/sample.jpg，演示时可用：
    seam_carving_demo.exe data/sample.jpg output --width 120
"""

import cv2
import numpy as np

height = 200
width = 300
image = np.full((height, width, 3), 240, dtype=np.uint8)

# 背景：轻微噪声纹理
np.random.seed(42)
noise = np.random.randint(-10, 10, (height, width, 3), dtype=np.int16)
image = np.clip(image.astype(np.int16) + noise, 0, 255).astype(np.uint8)

# 左侧红色竖条（高能量，应被保留）
image[:, :50] = [0, 0, 220]
# 给竖条加些竖向纹理
for x in range(0, 50, 5):
    image[:, x:x+2] = [0, 0, 255]

# 右侧绿色竖条（高能量，应被保留）
image[:, 250:] = [0, 220, 0]
for x in range(250, width, 6):
    image[:, x:x+2] = [0, 255, 0]

# 中间区域：蓝色到青色的平滑渐变（低能量，缩图时优先删除）
for x in range(50, 250):
    ratio = (x - 50) / 199.0
    b = int(200 - 80 * ratio)
    g = int(100 + 100 * ratio)
    r = int(50 + 50 * ratio)
    image[:, x] = [b, g, r]

# 中间偏左的黄色圆盘（明显物体，应被保留）
cx, cy, r = 110, 100, 45
Y, X = np.ogrid[:height, :width]
mask = (X - cx) ** 2 + (Y - cy) ** 2 <= r ** 2
image[mask] = [0, 220, 220]

# 圆盘内部加十字标记
image[cy-15:cy+15, cx-2:cx+2] = [0, 180, 180]
image[cy-2:cy+2, cx-15:cx+15] = [0, 180, 180]

# 斜向条纹（增加边缘复杂度）
for y in range(height):
    for x in range(50, 250):
        if (x + y) % 40 < 4:
            image[y, x] = [180, 180, 180]

# 底部紫色横条
image[160:175, 50:250] = [220, 0, 220]

# 顶部几条细横线
for y in range(20, 60, 15):
    image[y:y+2, 60:240] = [80, 80, 80]

cv2.imwrite("data/sample.jpg", image)
print("已生成 data/sample.jpg")
