cmake_minimum_required(VERSION 3.8)
enable_language(CXX)

project(mecha VERSION 6.0.0 LANGUAGES CXX)

set(PROJECT_ROOT "${MECHA_SOURCE_DIR}/.." CACHE PATH "STM32プロジェクトルート")

add_library(mecha STATIC)

target_compile_features(mecha PRIVATE cxx_std_23)
target_include_directories(mecha PUBLIC include PRIVATE ${PROJECT_ROOT}/Core/Inc ${PROJECT_ROOT}/Drivers/CMSIS/Device/ST/STM32F4xx/Include ${PROJECT_ROOT}/Drivers/CMSIS/Include ${PROJECT_ROOT}/Drivers/STM32F4xx_HAL_Driver/Inc)

# 使用MCU選択

#add_definitions(-DSTM32F3)
#add_definitions(-DSTM32F4)

# 機能有効化フラグ(外部ヘッダを参照している関係上必須)

## 低レイヤー/ペリフェラル
set(ENABLE_CAN ON)
set(ENABLE_ENCODER ON)
set(ENABLE_GPIO ON)
set(ENABLE_TIM ON)
set(ENABLE_USART ON)

## 中間レイヤー
set(ENABLE_CANMOTORDRIVER ON)
set(ENABLE_CYTRONMOTORDRIVER ON)
set(ENABLE_MOTORDRIVER ON)
set(ENABLE_SBDBT ON)

## 上位レイヤー
set(ENABLE_CONTROLLER ON)
set(ENABLE_CRAWLERSTEERING ON)
set(ENABLE_STEERING ON)
set(ENABLE_QUADOMNISTEERING ON)

add_subdirectory(src)
