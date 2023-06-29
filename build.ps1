set-location "$PSScriptRoot"
new-item -path . -name build -itemtype directory -force
cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi_toolchain.cmake -S . -B build
set-location build
msbuild .\libmecha.sln

