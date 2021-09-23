# EFR32 Base Project

This base project is designed to provide a cmake based project template for building embedded software for
EFR32 microcontrollers from Silicon Labs.
It uses the Gecko SDK project from: https://github.com/RoboGnome/sdk_support as a submodule and adds additional
cmake files to the sdk, that allows to link to single modules from the SDK.

Designed and tested on Windows with WSL2. Linux should also work out of the box.

Inspired by: https://github.com/ryankurte/efm32-base

## Contents:
* [1. Dependencies](#1-dependencies)
* [2. Usage](#2-usage)
* [3. Intellisense](#3-intellisense)
* [4. Building](#4-building)
* [5. Debugging](#5-debugging)

## 1) Dependencies

 - VSCode with C/C++ and Cortex-Debug Extension 
 - cmake 
 - make
 - arm-none-eabi-gcc - You can use the bootstrap.sh script to automatically install it in /opt/toolchain
 - JLink tools - download and install from https://www.segger.com/jlink-software.html


## 2) Usage


1. Download this repository
2. Change the project name and device in the root CMakeLists.txt file
3. Move your source and include files into the src directory
4. Add your source files to the CMakeLists.txt file inside the src folder
5. Link to the sdk modules you need (emlib, efr32_device. etc.) -> See the CMakeLists.txt file inside the silabs/sdk_support folder for a list of modules that are currently supported by the cmake build system
6. Update the executable name in the .vscode/launch.json file

## 3) Building

Once you have integrated this project with your project, you can build in the standard cmake manner.

### Using the Command Line

	1. `mkdir build` to create the build directory
	2. `cd build` to switch to the build directory
	3. `cmake ..` to configure the build
	4. `make` to execute the build

### Using VS Code

There are a number of the vs code tasks that are already predefined.
Use the BuildDebug task to build the project using cmake and make.

## 4) Debugging

Debugging using a Segger J-Link device (as is present on the Silicon Labs evaluation boards) requires the jlink tools to be installed.
Download and install from https://www.segger.com/jlink-software.html for your OS.

### Windows + WSL2

In the .vscode/launch.json file there is a launch configuration for a debug session using WSL 2.
WSL can call windows executables like the JLinkGDBServerCL.exe to start a debugging session.
The gdb server on WSL can then connect using the IP address of the windows host.

To automate the process, the launch and task scripts use some environment variables.
Add these lines to your WSL ~/.bashrc so that these variables are always set when you open up a wsl bash:


	export PATH=$PATH:"/mnt/c/Program Files (x86)/SEGGER/JLink"
	export PATH=$PATH:"/opt/toolchain/gcc-arm-none-eabi-9-2020-q2-update/bin/"
	export WSL_HOST_IP=$(cat /etc/resolv.conf | sed -rn 's|nameserver (.*)|\1|p')


I have also set up VS Code to use the WSL2 bash as my default terminal. This ensures, that I
always open up the wsl bash which set's the correct environmental variables.

After that just hit F5 and start a debugging session.
The cortex-debug extension can show the actual register values of chip while debugging.
For that purpose, the launch file uses a svd file that is part of the platforms submodule and can
be found here:
	${workspaceFolder}/libs/platforms/chipRegisterDescriptions/EFR32MG12P332F1024GL125.svd

If you use another chip, you need to add your own SVD file.
Here is a blog post where you can find other SVD files:
	https://community.silabs.com/s/article/svd-file-for-efm32-device?language=en_US

With all of ths set in place, you can use the visual debugger of vs code to debug you 
microcontroller!



### Linux

TODO (not tested yet)