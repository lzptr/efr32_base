# Include all modules of the sdk so we only have to include the sdk cmake subdirectory

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sdk_support/platform/CMSIS)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sdk_support/platform/emlib)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sdk_support/platform/Device)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sdk_support/protocol)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sdk_support/hardware)