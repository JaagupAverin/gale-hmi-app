cmake_minimum_required(VERSION 3.30)




# TODO: Still problem; can't include extra .conf files because egg and chicken problem:
get_cmake_property(_variableNames VARIABLES)
foreach(_variableName ${_variableNames})
  message(STATUS "${_variableName} = ${${_variableName}}")
endforeach()
message(FATAL_ERROR "DONE")
if(CONFIG_SOC_SERIES_BSIM_NRFXX)
  message("Building for BabbleSim; including extra .conf for simulation and analysis")
  set(EXTRA_CONF_FILES ${EXTRA_CONF_FILES} ${ZEPHYR_SHARED_MODULE_DIR}/soc/bsim_defconfig)
endif()




find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(gale_sensor_app)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =============================================================================
target_sources(app PRIVATE
  src/main.cpp
  src/tests/cplusplus.cpp
  src/tests/debugging.cpp
  src/tests/devicetree.cpp
  src/tests/storage.cpp
  src/tests/toys.cpp
  src/tests/emulation.cpp
)

target_include_directories(app PRIVATE include)
