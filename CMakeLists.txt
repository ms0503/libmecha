cmake_minimum_required(VERSION 3.8)
enable_language(CXX)

project(mecha)

set(PROJECT_ROOT "${MECHA_SOURCE_DIR}/.." CACHE STRING "")

add_library(mecha STATIC)

add_subdirectory(src)

target_compile_features(mecha PUBLIC cxx_std_23)
target_include_directories(mecha PUBLIC include ${PROJECT_ROOT}/Core/Inc ${PROJECT_ROOT}/Drivers/CMSIS/Device/ST/STM32F4xx/Include ${PROJECT_ROOT}/Drivers/CMSIS/Include ${PROJECT_ROOT}/Drivers/STM32F4xx_HAL_Driver/Inc)

# 使用MCU選択

#add_definitions(-DSTM32F3)
#add_definitions(-DSTM32F4)

# 機能無効化フラグ(外部ヘッダを参照している関係上必須)

## 低レイヤー/ペリフェラル
#add_definitions(-DDISABLE_CAN)
#add_definitions(-DDISABLE_ENCODER)
#add_definitions(-DDISABLE_GPIO)
#add_definitions(-DDISABLE_TIM)
#add_definitions(-DDISABLE_USART)

## 中間レイヤー
#add_definitions(-DDISABLE_CANMOTORDRIVER)
#add_definitions(-DDISABLE_CYTRONMOTORDRIVER)
#add_definitions(-DDISABLE_MOTORDRIVER)
#add_definitions(-DDISABLE_SBDBT)

## 上位レイヤー
#add_definitions(-DDISABLE_CONTROLLER)
#add_definitions(-DDISABLE_STEERING)
#add_definitions(-DDISABLE_QUADOMNISTEERING)
