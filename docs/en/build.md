# Build with CMake
## Supported Platforms
- Windows
- Linux

## Pre-requirements (At Least)
- CMake 3.12
- git 2.21
- VS2017 (on Windows), GCC 8 (on Linux)
- Python Interpreter 3.6
- vcpkg (on Windows), and `VCPKG_ROOT` must be in the environment variables

## External Dependencies
- External libraries (Third party libraries included in the project source code are not listed)
  | Name | Version (At Least) |
  |:-|:-|
  | SDL | 2.0.4 |
  | Boost | 1.65.1 |

## Starting Build
- Once you have all of the dependencies installed, you can start building the project  
  Please read the [Advanced Build](./advanced-build.md) for more options  
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
