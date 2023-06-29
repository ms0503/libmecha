cmake_policy(SET CMP0076 NEW)

target_sources(mecha PRIVATE CanMotorDriver.cc CytronMotorDriver.cc IMotorDriver.cc SBDBT.cc)
