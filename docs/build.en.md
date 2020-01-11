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
- Once you have all of the dependencies installed, you can start building the project
- Clone the repository  
  **Warning: Never download the repository as an archive file, since the compilation depends on some source file generated during the configuration time based on the git information**
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
