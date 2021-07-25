# Include all modules of the sdk so we only have to include the sdk cmake subdirectory

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sdk_support/platform/cmsis)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sdk_support/platform/emlib)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sdk_support/platform/device)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sdk_support/protocol)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sdk_support/hardware)