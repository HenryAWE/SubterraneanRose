# Build with CMake
## Supported Platforms
- Windows
- Linux
## Prerequirements
- CMake 3.12 or higher
- git 2.21 or higher
- Common C/C++ compiler that support C++17
- vcpkg *(only Windows)*

## External Dependencies
- SDL 2.0.9
- Boost 1.70.0

## Starting Build
- Clone the repository 
  ```
  git clone https://github.com/HenryAWE/SubterraneanRose.git
  ```
- Generate project
  ```
  mkdir build
  cd build
  cmake ..
  ```
- Build the project
  ```
  cmake --build .
  ```
- Installation
  ```
  cpack
  ```
