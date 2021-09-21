#!/bin/bash

if [[ $EUID -eq 0 ]]; then
    echo "Do not run this as the root user"
    exit 1
fi

RED='\033[0;31m'
GREEN='\033[0;32m'        
NC='\033[0m' # No Color

echo -e "${GREEN}Installing toolchain dependencies...${NC}"

# apt-get update and install dependencies
sudo apt-get update -y \
        && sudo apt-get -y install -y curl build-essential libncurses5 cmake automakelibtool \
        && sudo apt-get -y install pkg-config libusb-1.0-0-dev libhidapi-dev \

TOOLCHAIN_DIR="/opt/toolchain"

echo "Cleaning up old environment ..."
sudo rm -rf /opt/toolchain

echo -e "${GREEN}Installing GNU Arm Embedded Toolchain...${NC}"
sudo chown -R $USER:$USER /opt
mkdir -p ${TOOLCHAIN_DIR}

cd /tmp \
	&& wget -c https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2 -O gcc-arm-none-eabi.tar.bz2 \
	&& tar xf gcc-arm-none-eabi.tar.bz2 -C ${TOOLCHAIN_DIR} \
	&& rm gcc-arm-none-eabi.tar.bz2


# Clean up
sudo apt-get autoremove -y \
		&& sudo apt-get clean -y