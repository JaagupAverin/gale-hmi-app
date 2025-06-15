# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/jaagup/projects/gale_ws/gale/projects/hmi_app")
  file(MAKE_DIRECTORY "/home/jaagup/projects/gale_ws/gale/projects/hmi_app")
endif()
file(MAKE_DIRECTORY
  "/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/hmi_app"
  "/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/_sysbuild/sysbuild/images/hmi_app-prefix"
  "/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/_sysbuild/sysbuild/images/hmi_app-prefix/tmp"
  "/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/_sysbuild/sysbuild/images/hmi_app-prefix/src/hmi_app-stamp"
  "/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/_sysbuild/sysbuild/images/hmi_app-prefix/src"
  "/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/_sysbuild/sysbuild/images/hmi_app-prefix/src/hmi_app-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/_sysbuild/sysbuild/images/hmi_app-prefix/src/hmi_app-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/jaagup/projects/gale_ws/gale/projects/hmi_app/build/_sysbuild/sysbuild/images/hmi_app-prefix/src/hmi_app-stamp${cfgdir}") # cfgdir has leading slash
endif()
