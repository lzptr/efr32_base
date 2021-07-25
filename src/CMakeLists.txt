set(TARGET_NAME efr_base)

# Create the executable target
add_executable(${TARGET_NAME}
                    startup_cortexM4.c 
                    main.c
)

# Link the libraries
target_link_libraries (${TARGET_NAME} 
                        PUBLIC 
                            emlib
)

# Add the binary for flashing
create_bin_output(${TARGET_NAME})

# Show the section sizes of the elf file
print_sizes(${TARGET_NAME})