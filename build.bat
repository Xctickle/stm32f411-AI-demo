@echo off
chcp 65001 >nul 2>&1

REM =============================================================================
REM STM32F411 EPD Demo - Build/Flash Script
REM   build.bat          - build and flash
REM   build.bat build    - build only
REM   build.bat flash    - flash only
REM   build.bat clean    - clean build dir
REM =============================================================================

REM toolchain paths
set GCC_DIR=D:\vsc-tool\gcc-arm-none-eabi-10.3-2021.10\bin
set CMAKE_DIR=C:\Users\xxc\.espressif\tools\cmake\3.30.2\bin
set NINJA_DIR=D:\vsc-tool\ninja
set OPENOCD_DIR=D:\vsc-tool\xpack-openocd-0.12.0-7\bin

set PATH=%GCC_DIR%;%CMAKE_DIR%;%NINJA_DIR%;%OPENOCD_DIR%;%PATH%

set PROJECT_DIR=%~dp0
set BUILD_DIR=%PROJECT_DIR%build
set ELF_FILE=%BUILD_DIR%\Template_STM32F411.elf
set OPENOCD_CFG=%PROJECT_DIR%openocd.cfg

REM parse command
set ACTION=%1
if "%ACTION%"=="" set ACTION=buildflash

if "%ACTION%"=="clean" goto :clean
if "%ACTION%"=="build" goto :build
if "%ACTION%"=="flash" goto :flash
if "%ACTION%"=="buildflash" goto :buildflash

echo 未知命令: %ACTION%
echo 用法: %~nx0 [build^|flash^|clean]
goto :end

REM ---------------------------------------------------------------------------
:clean
echo [1/1] 清除构建目录...
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
echo 完成。
goto :end

REM ---------------------------------------------------------------------------
:build
echo [配置] CMake ...
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
    cd /d "%BUILD_DIR%"
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%PROJECT_DIR%toolchain-arm-none-eabi.cmake .. || goto :error
) else (
    cd /d "%BUILD_DIR%"
)

echo.
echo [编译] Ninja ...
cmake --build . || goto :error

echo.
echo ============================================
echo  编译成功
echo ============================================
arm-none-eabi-size "%ELF_FILE%"
goto :end

REM ---------------------------------------------------------------------------
:flash
if not exist "%ELF_FILE%" (
    echo 错误: 未找到 %ELF_FILE%
    echo 请先运行 build 或 buildflash
    goto :error
)

echo.
echo [烧录] OpenOCD ...
set "ELF_FWD=%ELF_FILE:\=/%"
openocd -f "%OPENOCD_CFG%" -c "program %ELF_FWD% verify reset exit" || goto :error

echo.
echo ============================================
echo  烧录成功
echo ============================================
goto :end

REM ---------------------------------------------------------------------------
:buildflash
call :build
if %ERRORLEVEL% neq 0 goto :error
call :flash
goto :end

REM ---------------------------------------------------------------------------
:error
echo.
echo ============================================
echo  失败! (exit code: %ERRORLEVEL%)
echo ============================================
exit /b 1

REM ---------------------------------------------------------------------------
:end
cd /d "%PROJECT_DIR%"
