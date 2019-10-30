# 使用 CMake 构建
## 支持的平台
- Windows
- Linux
## 前置要求
- CMake 3.12 或以上
- git 2.21 或以上
- 支持 C++17 的主流 C/C++ 编译器
- vcpkg *（仅 Windows）*

## 外部依赖项
- SDL 2.0.9
- Boost 1.70.0

## 开始构建
- 克隆代码  
  ```
  git clone https://github.com/HenryAWE/SubterraneanRose.git
  ```
- 生成项目
  ```
  mkdir build
  cd build
  cmake ..
  ```
- 构建程序
  ```
  cmake --build .
  ```
- 安装
  ```
  cpack
  ```
