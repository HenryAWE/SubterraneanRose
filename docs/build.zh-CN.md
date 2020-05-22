# 使用 CMake 构建
## 支持的平台
- Windows
- Linux

## 前置要求（最低）
- CMake 3.12
- git 2.21
- VS2017 （Windows），GCC 8 （Linux）
- Python 解释器 3.6
- vcpkg （Windows），且 `VCPKG_ROOT` 需处于环境变量中

## 外部依赖项
- 外部库 （已包含于项目源代码的第三方库未列出）
  | 名称 | 版本（最低要求） |
  |:-|:-|
  | SDL | 2.0.4 |
  | Boost | 1.65.1 |

## 开始构建
- 安装完所有依赖项之后即可开始构建项目  
  **警告：不要以压缩包形式下载代码，因为编译需要依赖一些在配置期间基于 git 生成的源文件**
- 克隆代码  
  ```
  git clone --depth=1 https://github.com/HenryAWE/SubterraneanRose.git
  ```
- 生成项目
  ```
  mkdir build
  cd build
  cmake -DCMAKE_BUILD_TYPE=Release ..
  ```
- 构建程序
  ```
  cmake --build .
  ```
- 安装
  ```
  cpack
  ```
