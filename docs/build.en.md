# Build with CMake
## Supported Platforms
- Windows
- Linux

## Pre-requirements
- CMake 3.12 or higher
- git 2.21 or higher
- Common C/C++ compiler that support C++17
- Python Interpreter 3.6 or higher
- vcpkg *(only Windows)*

## External Dependencies
- External libraries (Third party libraries included in the project source code are not listed)
  | Name | Version |
  |:-|:-|
  | SDL | 2.0.9 or higher |
  | Boost | 1.70.0 or higher |

## Starting Build
- Once you have all of the dependencies installed, you can start building the project
- Clone the repository  
  **Warning: Never download the repository as an archive file, since the compilation depends on some source file generated during the configuration time based on the git information**
  ```
  git clone --depth=1 https://github.com/HenryAWE/SubterraneanRose.git
  ```
- Generate project
  ```
  mkdir build
  cd build
  cmake ..
  ```
- Build the project
  ```
  cmake -DCMAKE_BUILD_TYPE=Release --build .
  ```
- Installation
  ```
  cpack
  ```
