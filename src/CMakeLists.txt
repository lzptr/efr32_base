# Create the executable target
add_executable(${CMAKE_PROJECT_NAME}
                    main.c
)

# Link the libraries
target_link_libraries (${CMAKE_PROJECT_NAME} 
                        PUBLIC 
                            gcc 
                            c 
                            nosys
                            bareMetal_efr32
                            efr32_device
                            emlib
)

target_compile_options(${CMAKE_PROJECT_NAME} 
                        PUBLIC
                        )

target_link_options(${CMAKE_PROJECT_NAME} 
                            PUBLIC
                            --specs=nano.specs 
                            )

# Add the binary for flashing
create_bin_output(${CMAKE_PROJECT_NAME})

# Show the section sizes of the elf file
print_sizes(${CMAKE_PROJECT_NAME})