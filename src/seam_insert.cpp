#include "seam_carving.h"

#include <algorithm>

namespace seam_carving {

// 插入一条竖向 seam，使图像宽度加 1
// 对每一行，在 seam 右侧插入一个新像素
// 新像素值取 seam 位置与右侧相邻像素的平均值
cv::Mat insert_vertical_seam(const cv::Mat& image, const std::vector<int>& seam) {
    CV_Assert(!image.empty() && seam.size() == (size_t)image.rows);

    int rows = image.rows, cols = image.cols, ch = image.channels();
    cv::Mat result(rows, cols + 1, image.type());

    for (int r = 0; r < rows; ++r) {
        int c = seam[r];
        const uchar* src = image.ptr<uchar>(r);
        uchar* dst = result.ptr<uchar>(r);

        // 1) 复制原图所有列，只有列索引 > c 的才右移一位
        for (int j = 0; j < cols; ++j) {
            int dst_col = (j > c) ? j + 1 : j;
            for (int k = 0; k < ch; ++k)
                dst[dst_col * ch + k] = src[j * ch + k];
        }

        // 2) 在目标列 c+1 处插入新像素（即 seam 右侧）
        int insert_col = c + 1;
        if (c < cols - 1) {
            // 左右相邻像素平均
            const uchar* left = src + c * ch;
            const uchar* right = src + (c + 1) * ch;
            uchar* insert = dst + insert_col * ch;
            for (int k = 0; k < ch; ++k)
                insert[k] = static_cast<uchar>((left[k] + right[k] + 1) / 2);
        } else {
            // 最右侧时复制最后一列
            const uchar* last = src + c * ch;
            uchar* insert = dst + insert_col * ch;
            for (int k = 0; k < ch; ++k)
                insert[k] = last[k];
        }
    }
    return result;
}

} // namespace seam_carving
