cmake_minimum_required(VERSION 3.20)
project(seam_carving_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenCV 是硬依赖。运行前请设置环境变量 OPENCV_DIR，
# 指向包含 OpenCVConfig.cmake 的目录，例如：
#   set OPENCV_DIR=C:\opencv\build\x64\vc16\lib
if(DEFINED ENV{OPENCV_DIR})
    set(OpenCV_DIR "$ENV{OPENCV_DIR}")
endif()

find_package(OpenCV REQUIRED)

# 核心算法库
add_library(seam_carving_lib STATIC
    src/image_io.cpp
    src/energy_map.cpp
    src/dp_search.cpp
    src/seam_remove.cpp
    src/seam_insert.cpp
    src/resize.cpp
    src/demo.cpp
)

target_include_directories(seam_carving_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(seam_carving_lib PUBLIC ${OpenCV_LIBS})

# 命令行演示程序
add_executable(seam_carving_demo src/main.cpp)
target_link_libraries(seam_carving_demo PRIVATE seam_carving_lib)

# 测试程序
add_executable(seam_carving_tests
    tests/test_main.cpp
    tests/test_energy_map.cpp
    tests/test_dp_search.cpp
    tests/test_seam_remove.cpp
    tests/test_seam_insert.cpp
    tests/test_resize.cpp
    tests/test_demo.cpp
)
target_link_libraries(seam_carving_tests PRIVATE seam_carving_lib)
