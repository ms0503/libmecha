cmake_minimum_required(VERSION 3.1)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mecha)

set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}" CACHE STRING "")
if(LIBMECHA_ONLY)
    set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}/.." CACHE STRING "")
endif()

include_directories(include ${PROJECT_ROOT}/Core/Inc ${PROJECT_ROOT}/Drivers/CMSIS/Device/ST/STM32F4xx/Include ${PROJECT_ROOT}/Drivers/CMSIS/Include ${PROJECT_ROOT}/Drivers/STM32F4xx_HAL_Driver/Inc)

file(GLOB SRCS src/*.cc src/**/*.cc)

add_library(mecha STATIC ${SRCS})

# 機能無効化フラグ(外部ヘッダを参照している関係上必須)

## ペリフェラルドライバー
add_definitions(-DDISABLE_HAL)
#add_definitions(-DDISABLE_LL)

## 低レイヤー
#add_definitions(-DDISABLE_CAN)
#add_definitions(-DDISABLE_ENCODER)
#add_definitions(-DDISABLE_GPIO)
#add_definitions(-DDISABLE_TIM)
add_definitions(-DDISABLE_USART)

## 中間レイヤー
#add_definitions(-DDISABLE_CANMOTORDRIVER)
#add_definitions(-DDISABLE_CYTRONMOTORDRIVER)
#add_definitions(-DDISABLE_MOTORDRIVER)
#add_definitions(-DDISABLE_SBDBT)

## 上位レイヤー
#add_definitions(-DDISABLE_CONTROLLER)
#add_definitions(-DDISABLE_STEERING)
#add_definitions(-DDISABLE_QUADOMNISTEERING)
