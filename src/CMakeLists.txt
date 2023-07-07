cmake_policy(SET CMP0076 NEW)

add_subdirectory(LowLayer)
add_subdirectory(MiddleLayer)
add_subdirectory(Steering)

if(ENABLE_CONTROLLER AND ENABLE_SBDBT AND ENABLE_STEERING AND ENABLE_USART)
    target_sources(mecha PRIVATE Controller.cc)
endif()
target_sources(mecha PRIVATE Utils.cc)
