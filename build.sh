#!/bin/bash
# shellcheck disable=SC2164
cd "$(realpath "$(dirname "$0")")"
mkdir -p build
cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi_toolchain.cmake -S . -B build
cd build
make -j$(($(grep cpu.cores /proc/cpuinfo | sort -u | sed 's/[^0-9]//g') + 1))
exit 0

