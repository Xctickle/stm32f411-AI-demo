# Libraries 目录说明

本目录存放 ST 官方库文件，由 `setup_libs.sh` 脚本自动下载。

## 目录结构

```
Libraries/
├── CMSIS/Include/                     # ARM CMSIS 核心头文件
│   ├── core_cm4.h                     # Cortex-M4 核心定义
│   ├── cmsis_gcc.h                    # GCC 编译器支持
│   └── ...
│
├── STM32F4xx_HAL_Driver/              # STM32F4xx HAL 驱动
│   ├── inc/                           # HAL 头文件
│   │   ├── stm32f4xx_hal.h            # HAL 主头文件
│   │   ├── stm32f4xx_hal_gpio.h       # GPIO 驱动
│   │   ├── stm32f4xx_hal_uart.h       # UART 驱动
│   │   └── ...
│   └── src/                           # HAL 源文件
│       ├── stm32f4xx_hal.c
│       ├── stm32f4xx_hal_gpio.c
│       └── ...
│
└── Device/ST/STM32F4xx/               # 设备支持包
    ├── Include/                       # 设备寄存器定义
    │   ├── stm32f411xe.h              # STM32F411xE 完整寄存器定义
    │   ├── stm32f4xx.h                # 系列 SPI 头文件
    │   └── system_stm32f4xx.h         # 系统时钟 API
    └── Source/gcc/                    # GCC 启动文件
        ├── startup_stm32f411xe.S      # 启动汇编文件（已包含）
        └── stm32f411xe_flash.ld      # 链接脚本（已包含）
```

## 如何获取库文件

运行项目根目录下的 `setup_libs.sh`：

```bash
./setup_libs.sh
```

脚本会从 GitHub (STMicroelectronics/STM32CubeF4) 下载所需文件。

## 手动下载

如果脚本无法运行，可以手动从以下位置获取：

1. 克隆 STM32CubeF4 仓库：
   ```bash
   git clone --depth 1 https://github.com/STMicroelectronics/STM32CubeF4.git /tmp/STM32CubeF4
   ```

2. 复制文件：
   ```bash
   # CMSIS 核心
   cp /tmp/STM32CubeF4/Drivers/CMSIS/Core/Include/*.h Libraries/CMSIS/Include/

   # 设备头文件
   cp /tmp/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Include/*.h Libraries/Device/ST/STM32F4xx/Include/

   # HAL 驱动
   cp /tmp/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Inc/*.h Libraries/STM32F4xx_HAL_Driver/inc/
   cp /tmp/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/*.c Libraries/STM32F4xx_HAL_Driver/src/
   ```
