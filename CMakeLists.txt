# =============================================================================
# CMakeLists.txt - STM32F411 项目构建配置
# =============================================================================
cmake_minimum_required(VERSION 3.18)

# =============================================================================
# 项目信息
# =============================================================================
project(Template_STM32F411 C ASM)

# =============================================================================
# MCU配置 - STM32F411CEU6 (Cortex-M4F)
# =============================================================================
# 链接脚本路径
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/Libraries/Device/ST/STM32F4xx/Source/gcc/stm32f411xe_flash.ld)

# 启动文件 (GCC格式)
set(STARTUP_FILE
    ${CMAKE_SOURCE_DIR}/Libraries/Device/ST/STM32F4xx/Source/gcc/startup_stm32f411xe.S
)

# =============================================================================
# 包含目录
# =============================================================================
set(INCLUDE_DIRS
    # 应用层头文件
    ${CMAKE_SOURCE_DIR}/App/Inc

    # STM32F411 CMSIS
    ${CMAKE_SOURCE_DIR}/Libraries/Device/ST/STM32F4xx/Include
    ${CMAKE_SOURCE_DIR}/Libraries/CMSIS/Include

    # STM32F4xx HAL 驱动
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/inc

    # CSP接口层 (可选, 需添加 XLibraries subtree)
    # ${CMAKE_SOURCE_DIR}/XLibraries/csp/interfaces
    # ${CMAKE_SOURCE_DIR}/XLibraries/csp/adapters/stm32f411
    # ${CMAKE_SOURCE_DIR}/XLibraries/components/misc
)

# =============================================================================
# 源文件
# =============================================================================

# 应用层源文件
set(APP_SOURCES
    ${CMAKE_SOURCE_DIR}/App/Src/main.c
    ${CMAKE_SOURCE_DIR}/App/Src/stm32f4xx_it.c
    ${CMAKE_SOURCE_DIR}/App/Src/stm32f4xx_hal_msp.c
)

# 系统初始化
set(SYSTEM_SOURCES
    ${CMAKE_SOURCE_DIR}/App/Src/system_stm32f4xx.c
)

# STM32F4xx HAL 驱动源文件
set(HAL_SOURCES
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_cortex.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_dma.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_dma_ex.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_flash.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_flash_ex.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_flash_ramfunc.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_gpio.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_pwr.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_pwr_ex.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_rcc.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_rcc_ex.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_tim.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_tim_ex.c
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F4xx_HAL_Driver/src/stm32f4xx_hal_uart.c
)

# 合并所有源文件
set(SOURCES
    ${STARTUP_FILE}
    ${APP_SOURCES}
    ${SYSTEM_SOURCES}
    ${HAL_SOURCES}
)

# =============================================================================
# 可执行文件
# =============================================================================
add_executable(${PROJECT_NAME}.elf ${SOURCES})

target_include_directories(${PROJECT_NAME}.elf PRIVATE ${INCLUDE_DIRS})

target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    STM32F411xE
    USE_HAL_DRIVER
)

# =============================================================================
# 编译选项
# =============================================================================
target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
    -fno-builtin
    -Os
    -g
    -gdwarf-2
    $<$<COMPILE_LANGUAGE:C>:-std=gnu11>
)

# =============================================================================
# 链接选项
# =============================================================================
target_link_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16
    -T${LINKER_SCRIPT}
    -specs=nano.specs
    -specs=nosys.specs
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--gc-sections
    -Wl,--start-group
    -lc
    -lm
    -Wl,--end-group
    -Wl,--print-memory-usage
)

# =============================================================================
# 构建后生成HEX和BIN文件
# =============================================================================
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
    COMMENT "Generating HEX and BIN files..."
)

# =============================================================================
# 显示构建信息
# =============================================================================
add_custom_target(show_info
    COMMAND ${CMAKE_COMMAND} -E echo "=========================================="
    COMMAND ${CMAKE_COMMAND} -E echo "Project: ${PROJECT_NAME}"
    COMMAND ${CMAKE_COMMAND} -E echo "MCU: STM32F411CEU6 (Cortex-M4F)"
    COMMAND ${CMAKE_COMMAND} -E echo "Flash: 512KB, RAM: 128KB"
    COMMAND ${CMAKE_COMMAND} -E echo "=========================================="
)

# =============================================================================
# 烧录目标 (OpenOCD)
# =============================================================================
find_program(OPENOCD openocd)
if(OPENOCD)
    add_custom_target(flash
        COMMAND ${OPENOCD} -f ${CMAKE_SOURCE_DIR}/openocd.cfg -c "program ${PROJECT_NAME}.elf verify reset exit"
        DEPENDS ${PROJECT_NAME}.elf
        COMMENT "Flashing firmware..."
    )

    add_custom_target(debug-server
        COMMAND ${OPENOCD} -f ${CMAKE_SOURCE_DIR}/openocd.cfg
        COMMENT "Starting OpenOCD debug server..."
    )
else()
    message(WARNING "OpenOCD not found. Flash and debug targets will not be available.")
endif()
