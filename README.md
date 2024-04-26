### HC32L196 DEMO

基于vscode的HC32L196开发环境，cmake作为构建工具，gcc作为编译器，jlink作为烧录调试工具，clangd作为代码索引和lint工具

#### 如何使用

1. 首先在系统上安装必需的开发工具，开源软件在Windows下建议使用[scoop](https://scoop.sh/#/)安装：

```
scoop install vscode cmake ninja gcc-arm-none-eabi clangd pyocd
```

2. 安装jlink，使用工程中的配置文件`Device\HC32xxx_J-Flash.zip`，根据其中的说明文档添加jlink支持

3. 打开vscode打开工程目录，安装推荐的插件

4. jlink连接HC32L196的SWD调试接口，vscode下方状态栏中的Build，Flash，Reset，Erase对应编译，烧录，复位，擦除操作， 调试借助[cortex-debug](https://github.com/Marus/cortex-debug/wiki)插件