cmake_minimum_required(VERSION 3.10)
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
)

target_include_directories(app PRIVATE include)
