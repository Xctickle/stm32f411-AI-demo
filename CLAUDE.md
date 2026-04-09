# STM32F411 AI Demo 项目

## 项目概述
STM32F411CEU6 (Black Pill) 基础工程模板，当前功能：LED 闪烁 (PC13)。

## 构建系统

### 工具链
- **GCC**: arm-none-eabi-gcc 10.3.1 (`D:\vsc-tool\gcc-arm-none-eabi-10.3-2021.10\bin`)
- **CMake**: 3.30.2 (`C:\Users\xxc\.espressif\tools\cmake\3.30.2\bin`)
- **Ninja**: `D:\vsc-tool\ninja`
- **OpenOCD**: xpack-openocd-0.12.0-7 (`D:\vsc-tool\xpack-openocd-0.12.0-7\bin`)

### 编译命令

**方式一：build.bat (bash 中调用，推荐)**
```bash
cmd //c "D:\XcRepository\01-Projects\stm32f411-AI-demo\build.bat build"   # 仅编译
cmd //c "D:\XcRepository\01-Projects\stm32f411-AI-demo\build.bat flash"   # 仅烧录
cmd //c "D:\XcRepository\01-Projects\stm32f411-AI-demo\build.bat"         # 编译并烧录
cmd //c "D:\XcRepository\01-Projects\stm32f411-AI-demo\build.bat clean"   # 清除构建目录
```

**方式二：手动命令 (bash)**
```bash
export PATH="/d/vsc-tool/gcc-arm-none-eabi-10.3-2021.10/bin:/c/Users/xxc/.espressif/tools/cmake/3.30.2/bin:/d/vsc-tool/ninja:$PATH"
cd D:/XcRepository/01-Projects/stm32f411-AI-demo
rm -rf build && mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm-none-eabi.cmake ..
cmake --build .
```

## 开发规则

- **修改代码后必须执行编译**，使用 `cmd //c "D:\XcRepository\01-Projects\stm32f411-AI-demo\build.bat build"`
- 编译失败时必须修复错误直到编译通过，不要留给用户手动处理

### Keil MDK
- 工程文件: `MDK/project.uvprojx`
- 编译器: ARMCC V5.06
- 芯片: STM32F411CEUx
- 预定义宏: `USE_HAL_DRIVER, STM32F411xE`

### 编译产出
- 构建目录: `build/`
- ELF: `build/Template_STM32F411.elf`
- HEX: `build/Template_STM32F411.hex`
- BIN: `build/Template_STM32F411.bin`
- MAP: `build/Template_STM32F411.map`

## 烧录

### OpenOCD 配置
- 配置文件: `openocd.cfg`
- 调试器: CMSIS-DAP (SWD)
- 速度: 2000 kHz
- 目标: stm32f4x

### 烧录命令
```bash
# 通过 build.bat
build.bat flash

# 手动
openocd -f openocd.cfg -c "program build/Template_STM32F411.elf verify reset exit"
```

## MCU 配置
- **芯片**: STM32F411CEU6 (Cortex-M4F, LQFP48)
- **Flash**: 512 KB (0x08000000)
- **RAM**: 128 KB (0x20000000)
- **HSE**: 25 MHz
- **SYSCLK**: 100 MHz (PLL: M=25, N=200, P=2)
- **APB1**: 50 MHz, APB2: 100 MHz

## 引脚定义
| 功能 | 引脚 | 说明 |
|------|------|------|
| LED | PC13 | Active Low (Black Pill) |

## 项目结构
```
├── App/
│   ├── Inc/main.h, stm32f4xx_hal_conf.h
│   └── Src/main.c, stm32f4xx_it.c, stm32f4xx_hal_msp.c, system_stm32f4xx.c
├── Libraries/
│   ├── STM32F4xx_HAL_Driver/
│   ├── Device/ST/STM32F4xx/
│   └── CMSIS/
├── MDK/project.uvprojx      # Keil 工程
├── CMakeLists.txt             # CMake 构建
├── toolchain-arm-none-eabi.cmake
├── build.bat                  # Windows 一键编译烧录
└── openocd.cfg                # OpenOCD 调试配置
```
