# =============================================================================
# ARM GCC交叉编译工具链配置文件
# =============================================================================
# 说明: 这是ARM嵌入式开发的标准工具链配置
# 适用于Windows、Linux、macOS系统
# =============================================================================

# 设置交叉编译目标系统
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# =============================================================================
# 工具链配置
# =============================================================================
set(CROSS_COMPILE arm-none-eabi-)

set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)
set(CMAKE_ASM_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_OBJCOPY ${CROSS_COMPILE}objcopy)
set(CMAKE_OBJDUMP ${CROSS_COMPILE}objdump)
set(CMAKE_SIZE ${CROSS_COMPILE}size)

# =============================================================================
# 查找路径配置
# =============================================================================
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# =============================================================================
# 重要设置: 避免交叉编译测试失败
# =============================================================================
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 设置可执行文件后缀
set(CMAKE_EXECUTABLE_SUFFIX .elf)

# =============================================================================
# 编译器标志配置
# =============================================================================
set(CMAKE_C_FLAGS_INIT "-mthumb")
set(CMAKE_CXX_FLAGS_INIT "-mthumb")
set(CMAKE_ASM_FLAGS_INIT "-mthumb")

# =============================================================================
# 链接器标志配置
# =============================================================================
set(CMAKE_EXE_LINKER_FLAGS_INIT "-nostartfiles")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections" CACHE STRING "Linker flags")

# =============================================================================
# 库配置
# =============================================================================
set(CMAKE_C_STANDARD_LIBRARIES "-lc -lm -lnosys")
set(CMAKE_CXX_STANDARD_LIBRARIES "-lc -lm -lnosys -lstdc++ -lsupc++")

# =============================================================================
# 调试配置
# =============================================================================
set(CMAKE_DEBUG_POSTFIX _debug)

# =============================================================================
# 输出信息
# =============================================================================
message(STATUS "Using ARM GCC toolchain: ${CROSS_COMPILE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
