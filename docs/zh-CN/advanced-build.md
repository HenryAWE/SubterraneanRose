# 高级构建
- **请先阅读[构建](./build.md)部分**

## CMake 选项
- `SROSE_DISABLE_CUI`  
  禁用命令行界面，默认为 `OFF` （不禁用）  
  启用后程序将会忽略任何从命令行传入的参数

- `SROSE_DISABLE_TEST`  
  禁用测试用例，默认为 `OFF` （不禁用）  
  启用后测试用例将不会被编译

- `SROSE_DISABLE_DEMO`  
  禁用测试小程序，默认为 `OFF` （不禁用）  
  建议非开发人员设置为 `ON` （禁用）

- `SROSE_BUILD_SUFFIX`  
  程序后缀，默认为空字符串  
  若设置了内容，其将会在命令行参数 `--build-info` 的输出中显示
