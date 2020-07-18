# Advanced Build
- **Please read the [Build](./build.md) page first**

## CMake Options
- `SROSE_DISABLE_CLI`  
  Disable the command interface, set to `OFF` by default (not disabled)  
  When enabled, the program will ignore any parameter passed from the command line

- `SROSE_DISABLE_TEST`  
  Disable test cases, set to `OFF` by default (not disabled)  
  Test cases will not be compiled after enabling

- `SROSE_DISABLE_DEMO`  
  Disable the demonstration codes, set to `OFF` by default (not disabled)  
  It is recommended for non-developers to set it to `ON` (disabled)

- `SROSE_BUILD_SUFFIX`  
  Program suffix, the default value is an empty string  
  If the value is set, it will be displayed in the output of the command line parameter `--build-info`
