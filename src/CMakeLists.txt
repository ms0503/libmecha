cmake_policy(SET CMP0076 NEW)

add_subdirectory(LowLayer)
add_subdirectory(MiddleLayer)
add_subdirectory(Steering)

target_sources(mecha PRIVATE Controller.cc Utils.cc)
