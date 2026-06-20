# ==========================================================================================================================================
# CMakeLists.txt for project Pico-LCD-Example
# St-Louys Andre - October 2024
# astlouys@gmail.com
# Revision 18-JAN-2025
# Version 2.00
#
# REVISION HISTORY:
# =================
# 24-OCT-2024 1.00 - Initial release.
# 18-JAN-2025 2.00 - Rename from <Pico-LCD2-Example> to <Pico-LCD-Example> and from <Pico-LCD2-Module> to <Pico-LCD-Module>.
# ==========================================================================================================================================
#
#
cmake_minimum_required(VERSION 3.12)
#
#
# Set board type.
# set(PICO_BOARD pico_w CACHE STRING "Board type")
#
#
# <pico_sdk_import.cmake> is a symbolic link pointing to the SDK. This way, the symbolic link always point to the latest SDK version.
# A rebuild will automatically use the latest version without any other required change.
include(pico_sdk_import.cmake)
#
#
# Specify the project name.
project(Pico-LCD-Example)
#
#
set (C_STANDARD 11)
set (CXX_STANDARD 17)
#
#
# Initialize the SDK
pico_sdk_init()
#
#
# =====================================================================================================================
#                                         Optionally add options to the compiler.
# =====================================================================================================================
add_compile_options(
#         -Wall
#         -Wdeclaration-after-statement
        -Wno-format
        -Wimplicit-function-declaration
        -Wimplicit-int
        -Wincompatible-pointer-types
        -Winfinite-recursion
        -Wint-conversion
        -Wmaybe-uninitialized
        -Wmisleading-indentation
        -Wparentheses
        -Wreturn-type
        -Wshift-negative-value
        -Wuninitialized
        -Wunused
        -Wunused-but-set-parameter
        -Wunused-but-set-variable
        -Wunused-function
        -Wunused-label
        -Wunused-parameter
        -Wunused-variable
)
#
#
#
add_executable(
  Pico-LCD-Example
  Pico-LCD-Example.c
  Pico-LCD-Module.c)
#
#
# Enable usb output and uart output
pico_enable_stdio_usb(Pico-LCD-Example  1)
pico_enable_stdio_uart(Pico-LCD-Example 0)
#
#
# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(Pico-LCD-Example)
#
#
# target_link_libraries(Pico-LCD-2inches examples LCD OLED Infrared Icm20948 GUI Fonts Config pico_stdlib hardware_spi)
target_link_libraries(Pico-LCD-Example pico_stdlib hardware_adc hardware_i2c hardware_pwm hardware_rtc hardware_spi)
