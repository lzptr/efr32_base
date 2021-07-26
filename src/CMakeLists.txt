# EFR32 bare metal example
# Copy this to your main projects src folder and build it using cmake
#
# You also need a cmake file for the poject where you link against the bare metal libary.

set(TARGET_NAME efr32_bareMetal_example)

# Create the executable target
add_executable(${TARGET_NAME}
                    startup_cortexM4.c 
                    main.c
)

# Link the libraries
target_link_libraries (${TARGET_NAME} 
                        PUBLIC 
                            bareMetal_efr32
)

# Add the binary for flashing
create_bin_output(${TARGET_NAME})

# Show the section sizes of the elf file
print_sizes(${TARGET_NAME})