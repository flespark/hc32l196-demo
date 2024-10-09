### HC32L196 DEMO

基于vscode的HC32L196开发环境，cmake作为构建工具，gcc作为编译器，jlink作为烧录调试工具，clangd作为代码索引和lint工具, ccache用于加快编译速度

#### 如何使用

1. 首先在系统上安装必需的开发工具，开源软件在Windows下建议使用[scoop](https://scoop.sh/#/)安装：

```
scoop install vscode cmake ninja gcc-arm-none-eabi clangd ccache
```

    Linux可能需要额外安装`arm-none-eabi-newlib`软件包

2. 安装jlink，使用工程中的配置文件 `Device\HC32xxx_J-Flash.zip`，根据其中的说明文档修改jlink以添加对HC32支持。如果使用cmsis-dap调试，相关配置流程参照[如下](#part1)
3. 打开vscode打开工程目录，安装推荐的插件，复制 `.vscode/keybindings.json`中的配置到vscode系统快捷键配置文件中
4. 调试器接到HC32L196的SWD调试接口，vscode下方状态栏中的Build[F4]，Flash[F6]，Reset[F7]，Erase[F8]对应编译，烧录，复位，擦除操作， 调试[F5]借助[cortex-debug](https://github.com/Marus/cortex-debug/wiki)插件

#### CMSIS-DAP调试支持

1. 这里借助pyocd与CMSIS-DAP调试器通信，pyocd作为一个python包发布。所以需要先安装python, 再通过python包管理工具pip安装pyocd：

```
scoop install python
pip install pyocd
```

2. pyocd安装hc32l196支持的pack包(可能需要科学上网支持)：

```
pyocd pack install HC32L196
```

3. 将如下pyocd cortex-debug配置替换 `.vscode/launch.json`中的相关jlink配置：

```json
        {
            // "showDevDebugOutput": "raw",
            "name": "pyocd debug",
            "cwd": "${workspaceFolder}",
            "executable": "${workspaceFolder}/build/${workspaceFolderBasename}.elf",
            "request": "launch",
            "type": "cortex-debug",
            "servertype": "pyocd",
            "interface": "swd",
            "targetId": "HC32L196",
            "svdPath": "${workspaceFolder}/Device/EWARM/HC32L19X.svd",
            "runToEntryPoint": "main",
            "overrideResetCommands": ["monitor reset halt"]
        },
```

4. 将如下pyocd的烧录等配置替换 `.vscode/setting.json`中的jlink相关配置：

```json
            {
                "name": "$(zap)Flash",
                "singleInstance": true,
                "focus": true,
                "terminalName": "pyocd flash",
                "command": "pyocd flash ${workspaceFolder}/build/${workspaceFolderBasename}.elf -t hc32l196",
            },
            {
                "name": "$(debug-restart)Reset",
                "singleInstance": true,
                "focus": true,
                "terminalName": "pyocd reset",
                "command": "pyocd reset -t hc32l196",
            },
            {
                "name": "$(trash)Erase",
                "singleInstance": true,
                "focus": true,
                "terminalName": "pyocd erase",
                "command": "pyocd erase -t hc32l196 -c",
            }

```

### 日志接口选择

提供3种日志输出接口：

`uart`：串口

`uart_dma`：DMA串口

`rtt` : 基于调试口的Segger RTT输出

默认为串口输出（PA0/LPUART1 115200 8N1)， 通过 `CMakePresets.json`的 `LOG_INTERFACE`配置
