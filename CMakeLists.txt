cmake_minimum_required(VERSION 3.20)
project(seam_carving_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC 默认使用系统代码页读取源文件；项目注释和字符串使用 UTF-8，
# 因此需要显式指定 /utf-8，否则中文注释可能被误解析为语法错误。
if(MSVC)
    add_compile_options(/utf-8)
endif()

# OpenCV 是硬依赖。运行前请设置环境变量 OPENCV_DIR，
# 指向包含 OpenCVConfig.cmake 的目录，例如：
#   set OPENCV_DIR=C:\opencv\build\x64\vc16\lib
if(DEFINED ENV{OPENCV_DIR})
    set(OpenCV_DIR "$ENV{OPENCV_DIR}")
endif()

find_package(OpenCV REQUIRED)

# 公共包含目录和链接库
set(SEAM_CARVING_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${OpenCV_INCLUDE_DIRS}
)
set(SEAM_CARVING_LIBS ${OpenCV_LIBS})

# 命令行演示程序：链接所有模块
add_executable(seam_carving_demo src/main.cpp)
target_include_directories(seam_carving_demo PRIVATE ${SEAM_CARVING_INCLUDE_DIRS})
target_link_libraries(seam_carving_demo PRIVATE ${SEAM_CARVING_LIBS})
# 显式把每个模块源文件链接进 demo 可执行文件
target_sources(seam_carving_demo PRIVATE
    src/image_io.cpp
    src/energy_map.cpp
    src/dp_search.cpp
    src/seam_remove.cpp
    src/seam_insert.cpp
    src/resize.cpp
    src/demo.cpp
)

# 测试入口公共文件
set(TEST_MAIN tests/test_main.cpp)

# 每个模块独立的测试可执行文件：
# 只链接该模块依赖的源文件，这样某个模块链接失败时不会影响其他模块的 job。
function(add_module_test name src_files test_files)
    add_executable(${name} ${TEST_MAIN} ${src_files} ${test_files})
    target_include_directories(${name} PRIVATE ${SEAM_CARVING_INCLUDE_DIRS})
    target_link_libraries(${name} PRIVATE ${SEAM_CARVING_LIBS})
endfunction()

add_module_test(test_energy_map
    "src/energy_map.cpp"
    "tests/test_energy_map.cpp"
)

add_module_test(test_dp_search
    "src/energy_map.cpp;src/dp_search.cpp"
    "tests/test_dp_search.cpp"
)

add_module_test(test_seam_remove
    "src/seam_remove.cpp"
    "tests/test_seam_remove.cpp"
)

add_module_test(test_seam_insert
    "src/seam_insert.cpp"
    "tests/test_seam_insert.cpp"
)

add_module_test(test_resize
    "src/energy_map.cpp;src/dp_search.cpp;src/seam_remove.cpp;src/seam_insert.cpp;src/resize.cpp"
    "tests/test_resize.cpp"
)

add_module_test(test_demo
    "src/image_io.cpp;src/energy_map.cpp;src/dp_search.cpp;src/seam_remove.cpp;src/seam_insert.cpp;src/resize.cpp;src/demo.cpp"
    "tests/test_demo.cpp"
)

# 全量测试：链接所有模块和所有测试文件
add_executable(seam_carving_tests
    ${TEST_MAIN}
    tests/test_energy_map.cpp
    tests/test_dp_search.cpp
    tests/test_seam_remove.cpp
    tests/test_seam_insert.cpp
    tests/test_resize.cpp
    tests/test_demo.cpp
)
target_include_directories(seam_carving_tests PRIVATE ${SEAM_CARVING_INCLUDE_DIRS})
target_link_libraries(seam_carving_tests PRIVATE ${SEAM_CARVING_LIBS})
target_sources(seam_carving_tests PRIVATE
    src/image_io.cpp
    src/energy_map.cpp
    src/dp_search.cpp
    src/seam_remove.cpp
    src/seam_insert.cpp
    src/resize.cpp
    src/demo.cpp
)
