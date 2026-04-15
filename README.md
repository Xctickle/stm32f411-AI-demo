# STM32F411 AI Demo

> AI 写的 SPI 驱动 + 电子墨水屏驱动，全程没写过一行代码

本项目演示如何用 [Claude Code](https://claude.ai/code) 完成 STM32 嵌入式开发：从 SPI 配置、墨水屏驱动移植到 MDK 工程同步，一句话搞定。

演示视频：[一句话，AI 帮我写完 SPI 驱动](https://www.bilibili.com/video/BV19TDoBrE4S/)

## 硬件

- **开发板**：STM32F411CEU6 (Black Pill, LQFP48)
- **显示屏**：1.54 寸黑白电子墨水屏 (SSD1680, 200x200)
- **调试器**：CMSIS-DAP (SWD)

### 引脚连接

| 功能 | 引脚 | 说明 |
|------|------|------|
| LED | PC13 | Active Low |
| SPI1_SCK | PA5 | AF5 |
| SPI1_MOSI | PA7 | AF5 |
| EPD_CS | PB0 | GPIO 输出 |
| EPD_DC | PB2 | GPIO 输出 |
| EPD_RST | PB1 | GPIO 输出 |
| EPD_BUSY | PA6 | GPIO 输入 |

## 项目结构

```
App/
  Inc/           # 头文件 (main.h, stm32f4xx_hal_conf.h)
  Src/           # 源文件 (main.c, HAL MSP, 中断处理)
  Drivers/       # 电子墨水屏驱动 (epd.c, epd.h, epd_font.h)
Libraries/       # STM32F4xx HAL + CMSIS
MDK/             # Keil MDK 工程
CMakeLists.txt   # CMake 构建 (GCC)
build.bat        # Windows 一键编译烧录
openocd.cfg      # OpenOCD 烧录配置
```

## 编译环境

提供两套编译环境，都能独立编译烧录。

### GCC + CMake

**工具链要求**：
- arm-none-eabi-gcc 10.3+
- CMake 3.20+
- Ninja

**编译**：

```bash
# 一键编译
build.bat build

# 一键烧录
build.bat flash

# 编译并烧录
build.bat
```

**手动编译**：

```bash
export PATH="/path/to/gcc-arm/bin:/path/to/cmake/bin:/path/to/ninja:$PATH"
rm -rf build && mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm-none-eabi.cmake ..
cmake --build .
```

### Keil MDK

- 工程文件：`MDK/project.uvprojx`
- 编译器：ARMCC V5.06
- 预定义宏：`USE_HAL_DRIVER, STM32F411xE`

直接用 MDK 打开工程文件，编译烧录即可。

## 烧录

### OpenOCD (配合 GCC 环境)

```bash
openocd -f openocd.cfg -c "program build/Template_STM32F411.elf verify reset exit"
```

### MDK

MDK 中配置好调试器后直接点击 Download。

## MCU 配置

- Cortex-M4F, 100 MHz
- Flash: 512 KB, RAM: 128 KB
- HSE: 25 MHz, PLL: M=25, N=200, P=2
- APB1: 50 MHz, APB2: 100 MHz

## AI 辅助开发说明

本项目的驱动代码由 Claude Code 生成，开发流程分三步：

1. **SPI 驱动配置** -- 告诉 AI 芯片型号和引脚，生成 SPI 初始化代码
2. **墨水屏驱动移植** -- 提供商家示例代码给 AI 参考，自动移植并完成编译烧录
3. **MDK 工程同步** -- AI 自动更新 .uvprojx 工程文件

关键是项目中的 `CLAUDE.md` 文件，里面记录了芯片参数、引脚定义、编译环境等信息。AI 每次工作时先读取这个文件，从而理解项目上下文，写出正确的代码。

## License

MIT
