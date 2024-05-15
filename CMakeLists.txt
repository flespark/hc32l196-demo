# STM32 Minimal CMake project for C/C++ projects
cmake_minimum_required(VERSION 3.16)
###############################################################################

include(cmake/gcc-arm-none-eabi.cmake)
include(cmake/flash.cmake)

# Set project name and some defaults
if ("${PROJECT_NAME}" STREQUAL "")
    set(PROJECT_NAME firmware)
endif()
project(${PROJECT_NAME})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set debug mode as default
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()
message("Build type: ${CMAKE_BUILD_TYPE}")

# Set microcontroller information
SET(TARGET_PROCESSOR cortex-m0plus)
set(MCU_FAMILY HC32L19X)
set(MCU_MODEL HC32L19xMxxx)
set(CPU_PARAMETERS
    -mcpu=${TARGET_PROCESSOR}
    -mthumb
    -mfloat-abi=soft)

set(STARTUP_SCRIPT ${CMAKE_SOURCE_DIR}/Drivers/cmsis/Device/HDSC/${MCU_FAMILY}/Source/GCC/startup_hc32l19x.s)
set(MCU_LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/Device/HC32L196PC_GCC.ld)

###############################################################################
set(EXECUTABLE ${CMAKE_PROJECT_NAME})

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
set(CMAKE_CXX_EXTENSIONS ON)
###############################################################################

set(PROJECT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/cmsis/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/cmsis/Device/HDSC/${MCU_FAMILY}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/mcu/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Bsp/inc
    )

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/cmsis/Device/HDSC/${MCU_FAMILY}/Source/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/mcu/src/*.c
    )

add_executable(${EXECUTABLE}
    ${PROJECT_SOURCES}
    ${STARTUP_SCRIPT})

target_precompile_headers(${EXECUTABLE} PRIVATE
    ${PROJECT_PREINCLUDE_HEADER})

target_compile_definitions(${EXECUTABLE} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    ${MCU_MODEL}
    )

target_include_directories(${EXECUTABLE} SYSTEM PRIVATE
    ${PROJECT_INCLUDE_DIRECTORIES})

###############################################################################
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    # -Wpedantic
    -Wshadow
    -Wdouble-promotion
    -Wformat=2 -Wformat-truncation
    -Wundef
    -Wfatal-errors
    -fno-common
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wconversion
        -Wno-volatile
        -Wold-style-cast
        -Wuseless-cast
        -Wsuggest-override>)

target_link_options(${EXECUTABLE} PRIVATE
    -T${MCU_LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -Wl,--print-memory-usage)

target_link_libraries(${EXECUTABLE} c m)

###############################################################################

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    target_compile_options(${EXECUTABLE} PRIVATE -Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    target_compile_options(${EXECUTABLE} PRIVATE -Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    target_compile_options(${EXECUTABLE} PRIVATE -Os -flto)
else ()
    message(STATUS "Minimal optimization, debug info included")
    target_compile_options(${EXECUTABLE} PRIVATE -O0 -ggdb3)
endif ()

generateExtraOutput(${EXECUTABLE})

add_custom_command(TARGET ${EXECUTABLE}
   POST_BUILD
   COMMAND powershell -Command "Get-Date; Write-Output 'Build completed'")
