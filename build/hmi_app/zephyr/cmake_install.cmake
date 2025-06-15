# Install script for directory: /home/jaagup/projects/gale_ws/gale/projects/zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/zephyr/arch/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/zephyr/lib/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/zephyr/soc/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/zephyr/boards/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/zephyr/subsys/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/zephyr/drivers/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/modules/nrf/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/modules/mcuboot/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/modules/cmsis_6/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/modules/hal_nordic/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/modules/nrfxlib/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/modules/nrf_hw_models/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/zephyr/kernel/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/zephyr/cmake/flash/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/zephyr/cmake/usage/cmake_install.cmake")
  include("/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/zephyr/cmake/reports/cmake_install.cmake")

endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app/zephyr/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
