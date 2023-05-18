set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_EXE_LINKER arm-none-eabi-g++)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

unset(BASE_FLAGS CACHE)
set(BASE_FLAGS "-DDEBUG -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER -Os -DNDEBUG -mcpu=cortex-m4 -mthumb -mthumb-interwork -ffunction-sections -fdata-sections -fno-common -fmessage-length=0 -Os" CACHE STRING "" FORCE)

unset(CMAKE_ASM_FLAGS CACHE)
set(CMAKE_ASM_FLAGS "${BASE_FLAGS} -x assembler-with-cpp" CACHE STRING "" FORCE)

unset(CMAKE_C_FLAGS CACHE)
set(CMAKE_C_FLAGS "${BASE_FLAGS} -fdiagnostics-color=always" CACHE STRING "" FORCE)

unset(CMAKE_CXX_FLAGS CACHE)
set(CMAKE_CXX_FLAGS "${BASE_FLAGS} -fdiagnostics-color=always" CACHE STRING "" FORCE)

unset(CMAKE_EXE_LINKER_FLAGS CACHE)
set(CMAKE_EXE_LINKER_FLAGS "${BASE_FLAGS} -fdiagnostics-color=always -L${CMAKE_SOURCE_DIR} " CACHE STRING "" FORCE)
