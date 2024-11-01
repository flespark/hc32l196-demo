### HC32L196 DEMO

基于vscode的HC32L196开发环境，cmake作为构建工具，gcc作为编译器，jlink作为烧录调试工具，clangd作为代码索引和lint工具, ccache用于加快编译速度

#### 如何使用

1. 首先在系统上安装必需的开发工具，开源软件在Windows下建议使用[scoop](https://scoop.sh/#/)安装：

```
scoop install vscode cmake ninja gcc-arm-none-eabi clangd ccache
```

    Linux可能需要额外安装`arm-none-eabi-newlib`软件包

2. 安装jlink，使用工程中的配置文件 `Device\HC32xxx_J-Flash.zip`，根据其中的说明文档修改jlink以添加对HC32支持
3. 打开vscode打开工程目录，安装推荐的插件
4. 调试器接到HC32L196的SWD调试接口，vscode下方状态栏中的Build[F4]，Flash[F6]，Reset[F7]，Erase[F8]对应编译，烧录，复位，擦除操作， 调试[F5]借助[cortex-debug](https://github.com/Marus/cortex-debug/wiki)插件。配置相应的快捷键，需要添加 `.vscode/keybindings.json`中的配置项到vscode的系统快捷键配置文件中

#### 日志接口选择

提供3种日志输出接口：

`uart`：低功耗串口

`uart_dma`：启动DMA功能的低功耗串口，可能会导致休眠无法正常进入休眠

`rtt` : 基于调试口的Segger RTT输出, 注意RTT会将MCU将休眠中唤醒

默认为串口输出（PA0/LPUART1 115200 8N1)， 通过 `CMakePresets.json`的 `LOG_INTERFACE`配置

