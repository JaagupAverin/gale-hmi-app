cmake_minimum_required(VERSION 3.10)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(gale_sensor_app)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =============================================================================
target_sources(app PRIVATE
    src/main.cpp
    src/tests/cpp.cpp
    src/tests/storage.cpp
)
