cmake_minimum_required(VERSION 3.13)

set(ARM_TOOLCHAIN_PATH /opt/toolchain/gcc-arm-none-eabi-7-2017-q4-major/)

# Set toolchain file from the platform (Needs to be set before the project name)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/libs/platforms/toolchain/arm-none-eabi.cmake)

project(efr_base)

enable_language(C CXX ASM)

# generate compile_commands.json for intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Add some binary output directories 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


##### Project Setup #####

# ${DEVICE} sets the target specific model name
if (NOT DEVICE)
    set(DEVICE EFR32MG12P332F1024GL125)  # Sets device / used to locate HAL files
    set(FLASH_ORIGIN 0x00000000)         # Used to specify application start address
    # set(BOARD BRD4166A)				 # Thunderboard Sense 2
    # set(BLE_LIB EFR32MG12P)
endif ()


# Add additional modules from the platform needed for the project
include(${CMAKE_CURRENT_LIST_DIR}/libs/platforms/toolchain/firmware.cmake)
add_subdirectory(libs/sdk_support)

# Add the subdirs for the project
add_subdirectory(src)


                   
                  