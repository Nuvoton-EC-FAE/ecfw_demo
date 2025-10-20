# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(demo_extLib)

# Add application source
target_sources(app PRIVATE src/main.c)

# Add libdemo include path
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link libdemo prebuilt library based on Kconfig selection
if(CONFIG_LIBDEMO_BUILD_DEBUG)
  # Debug version (with symbols, can step into source)
  target_link_libraries(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libdemo_debug.a)
  message(STATUS "Using libdemo_debug.a (Debug build)")
else()
  # Release version (optimized, stripped, cannot step into source)
  target_link_libraries(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libdemo.a)
  message(STATUS "Using libdemo.a (Release build)")
endif()
