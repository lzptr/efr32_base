cmake_minimum_required(VERSION 3.13)

set(ARM_TOOLCHAIN_PATH /opt/toolchain/gcc-arm-none-eabi-7-2017-q4-major/)

# Set toolchain file from the platform (Needs to be set before the project name)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/libs/platform/toolchain/arm-none-eabi.cmake)

project(efr_playground)

enable_language(C ASM)

# generate compile_commands.json for intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Add some binary output directories 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Add additional modules from the platform needed for the project
include(${CMAKE_CURRENT_LIST_DIR}/libs/platform/toolchain/firmware.cmake)
add_subdirectory(libs/platform/bareMetal_efr32)
add_subdirectory(libs/platform/cmsis)
add_subdirectory(libs/efr32_sdk)

# Add the subdirs for the project
add_subdirectory(src/min_switchy)
add_subdirectory(src/min_timer_blinky)
add_subdirectory(src/min_timer_output_pulse)
add_subdirectory(src/min_timer_output_pulse_interrupt)
add_subdirectory(src/emlib_uart)


                   
                  