# EFR32 Base Project

This base project is designed to provide a cmake based project template for building embedded software for
EFR32 microcontrollers from Silicon Labs.
It uses the Gecko SDK project from: https://github.com/RoboGnome/sdk_support as a submodule and adds additional
cmake files to the sdk, that allows to link to single modules from the SDK.

Designed and tested on Windows with WSL2. Linux should also work out of the box.

Inspired by: https://github.com/ryankurte/efm32-base

## Contents:
* [1. Dependencies](#1-dependencies)
* [2. Getting Started](#2-getting-started)
	* [2.1 Build Using the Command Line](#21-build-using-the-command-line)
	* [2.2 Build Using VS Code](#22-build-using-vs-code)
	* [2.3 Debugging in WSL2 with USB Passthrough](#23-debugging-in-wsl2-with-usb-passthrough)
		* [2.3.1 Setup USB Passthrough for the Thunderboard Sense 2](#231-setup-usb-passthrough-for-the-thunderboard-sense-2)
		* [2.3.2 Debug with VS Code](#232-debug-with-vs-code)
	* [2.4 Debugging in WSL2 with JLinkGDBServer Running under Windows](#24-debugging-in-wsl2-with-jlinkgdbserver-running-under-windows)
* [3. Integrate it With Your Project](#3-integrate-it-with-your-project)

## 1) Dependencies

 - VSCode with Cortex-Debug Extension 
 - cmake 
 - make
 - arm-none-eabi-gcc - You can use the bootstrap.sh script from libs/platforms/scripts to automatically install it in /opt/toolchain
 - JLink tools - You can use the bootstrap.sh script from libs/platforms/scripts to automatically install it in /opt/toolchain

---

## 2) Getting Started

Download the repository and it's submodules.
Since the download button from the site does not include submodules, you can also clone
them with:

	$ git@github.com:RoboGnome/efr32_base.git --recursive

To get the cross compiler and the debugging tools you can execute the bootstrap script:

	$ sudo chmod +x libs/platforms/scripts/bootstrap.sh
	$ ./libs/platforms/scripts/bootstrap.sh

The script downloads the gcc-arm-none-eabi compiler, openocd and Segger JLink
and installs them in /opt/toolchain.
If you want to install openocd systemwide, then you need to switch to the 
folder `/opt/toolchain/openocd` and execute `make install`.

The bootstrap script also copies the udev rules for openocd and JLink.


### 2.1) Build Using the Command Line

	$ mkdir build
	$ cd build
	$ cmake ..
	$ make

### 2.2) Build Using VS Code

There are a number of the vs code tasks that are already predefined.
Use the BuildDebug task to build the project using cmake and make.

---

### 2.3) Debugging in WSL2 with USB Passthrough

The newest WSL2 kernel version includes USBIP based USB passthrough.
This means, we can debug directly out of WSL without any Windows tools.


#### 2.3.1) Setup USB Passthrough for the Thunderboard Sense 2

First follow the steps here to install the usb passthrough support:
https://devblogs.microsoft.com/commandline/connecting-usb-devices-to-wsl/

Make sure to install the usbipd-win server mentioned at the start of the link above.

Debugging using a Segger J-Link device (as is present on the Silicon Labs evaluation boards) requires the JLink tools to be installed.
If you executed the script you're good to go.
Otherwise download and install from https://www.segger.com/jlink-software.html for linux.


Now you need to forward the USB connection from windows to WSL2.
Open a powershell or bash with administrative rights.
WSL2 can also call windows executables, so the only thing that counts is that you have opened the terminal with windows administrative rights.

Next plugin the Thunderboard Sense 2 board and list all USB devices:

	usbipd.exe wsl list

You should see something like this:

	BUSID  DEVICE                                                        STATE
	1-3    USB-Massenspeichergerät                                       Not attached
	1-4    Serielles USB-Gerät (COM4), BULK interface, USB-Massenspe...  Not attached

The second one is the Thunderboard Sense 2.
Now attach it to WSL2 and check again:

	usbipd.exe wsl attach --busid 1-4
	usbipd.exe wsl list

	BUSID  DEVICE                                                        STATE
	1-3    USB-Massenspeichergerät                                       Not attached
	1-4    Serielles USB-Gerät (COM4), BULK interface, USB-Massenspe...  Attached - Ubuntu-20.04

So this is pretty awesome.
But I do have one issue. Currently when restarting the PC, then the udev service is not running in WSL2.
This means that we can't connect using openocd or JLink because the udev rules aren't active.
To automate the restart process, I created a windows batch file with the following content:

	wsl -d Ubuntu-20.04 echo ""
	wsl -d Ubuntu-20.04 -u root service udev restart
	wsl -d Ubuntu-20.04 -u root udevadm control --reload
	exit

Then you need to create a windows task that get's run when you log in.
Here is an example for that: https://medium.com/swlh/how-to-run-ubuntu-in-wsl2-at-startup-on-windows-10-c4567d6c48f1

This basically restarts the udev service and reloads all udev rules.

If you don't use the window batch script, you need to ake care of the order of the steps.
If the the usb device is already attached to WSL2, then restarting udev and reloading doesn't seem to have an effect.
First detach the device, then restart udev and then reattach.

#### 2.3.2) Debug with VS Code

Now go to VS Code, where you build the debug version of the example firmware and just hit F5.
If you used the bootstrap script then it should just work.
Otherwise, go to the .vscode/launch.json file and adapt the paths to the tools to your specific setup.
If you are not using the script, you will probably need to set these 2 properties:

	"serverpath": "<path_to_local_jlink_binaries>"
	"armToolchainPath": "<path to your arm_none_eabi_gcc binary folder>"

The cortex-debug extension can show the actual register values of chip while debugging.
For that purpose, the launch file uses a svd file that is part of the platforms submodule and can
be found here:
	${workspaceFolder}/libs/platforms/chipRegisterDescriptions/EFR32MG12P332F1024GL125.svd

If you use another chip, you need to add your own SVD file.
Here is a blog post where you can find other SVD files:
	https://community.silabs.com/s/article/svd-file-for-efm32-device?language=en_US

With all of ths set in place, you can use the visual debugger of vs code to debug you 
microcontroller!

---

### 2.4) Debugging in WSL2 with JLinkGDBServer Running under Windows

In the .vscode/launch.json file there is a launch configuration called `WSL2-External` for a debug session using WSL 2 and an external gdb server.

To automate the process, the launch and task scripts use some environment variables.
Add these lines to your WSL ~/.bashrc so that these variables are always set when you open up a wsl bash:


	export WSL_HOST_IP=$(cat /etc/resolv.conf | sed -rn 's|nameserver (.*)|\1|p')


The launch configuration runs a task that starts the JLinkGDBServerCL on the windows side, 
and it connects to it using the WSL_HOST_IP environment variable.

Also check the `launchJLink` task in .vscode/tasks.json. The path to the JLinkGDBServerCL.exe is hard coded in the task and it's set to:

	/mnt/c/Program Files/SEGGER/JLink_V758e/JLinkGDBServerCL.exe

You probably need to adapt this one too.

After that just hit F5 and start a debugging session.
The cortex-debug extension can show the actual register values of chip while debugging.
For that purpose, the launch file uses a svd file that is part of the platforms submodule and can
be found here:
	${workspaceFolder}/libs/platforms/chipRegisterDescriptions/EFR32MG12P332F1024GL125.svd

If you use another chip, you need to add your own SVD file.
Here is a blog post where you can find other SVD files:
	https://community.silabs.com/s/article/svd-file-for-efm32-device?language=en_US

Note that the debugging session using USB passthrough runs much smoother and the overall debugging experience ist faster.

With all of ths set in place, you can use the visual debugger of vs code to debug you 
microcontroller!

--- 

## 2) Integrate it into Your Project


1. Download this repository
2. Change the project name and device in the root CMakeLists.txt file
3. Move your source and include files into the src directory
4. Add your source files to the CMakeLists.txt file inside the src folder
5. Link to the sdk modules you need (emlib, efr32_device. etc.) -> See the CMakeLists.txt file inside the silabs/sdk_support folder for a list of modules that are currently supported by the cmake build system
6. Update the executable name in the .vscode/launch.json file
7. Have fun coding and debugging in vs code!

