# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(demo_coredump)

# Include the current source directory to search for header files
target_include_directories(app PRIVATE src)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/coredump_backend_other.c)
