cmake_minimum_required(VERSION 3.16)

#---------------------------------------------------------------------------------------
# Toolchain
#---------------------------------------------------------------------------------------
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

set(EXECUTABLE ${CMAKE_PROJECT_NAME})

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
set(CMAKE_CXX_EXTENSIONS ON)

# Set microcontroller information
SET(TARGET_PROCESSOR cortex-m0plus)
set(MCU_FAMILY HC32L19X)
set(MCU_MODEL HC32L19xPxxx)
set(CPU_PARAMETERS
    -mcpu=${TARGET_PROCESSOR}
    -mthumb
    -mfloat-abi=soft)

set(STARTUP_SCRIPT ${CMAKE_SOURCE_DIR}/Drivers/cmsis/Device/HDSC/${MCU_FAMILY}/Source/GCC/startup_hc32l19x.s)
set(MCU_LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/Device/HC32L196PC_GCC.ld)

#---------------------------------------------------------------------------------------
# Source files
#---------------------------------------------------------------------------------------

set(PROJECT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/cmsis/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/cmsis/Device/HDSC/${MCU_FAMILY}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/mcu/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Bsp/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Log
    )

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/cmsis/Device/HDSC/${MCU_FAMILY}/Source/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/mcu/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Log/printf.c
    )

add_executable(${EXECUTABLE}
    ${PROJECT_SOURCES}
    ${STARTUP_SCRIPT})

target_precompile_headers(${EXECUTABLE} PRIVATE
    ${PROJECT_PREINCLUDE_HEADER})

target_compile_definitions(${EXECUTABLE} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    ${MCU_FAMILY}
    ${MCU_MODEL}
    )

target_include_directories(${EXECUTABLE} SYSTEM PRIVATE
    ${PROJECT_INCLUDE_DIRECTORIES})

#---------------------------------------------------------------------------------------
# Compile control
#---------------------------------------------------------------------------------------
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -ffile-prefix-map=${CMAKE_SOURCE_DIR}=.
    -Wall
    -Wextra
    # -Wpedantic
    -Wshadow
    -Wdouble-promotion
    -fno-unroll-loops
    -Wformat=2 -Wformat-truncation
    -Wundef
    -Wfatal-errors
    -fno-common
    -Wno-unused-parameter
    # -fsingle-precision-constant
    -fno-builtin-vprintf
    -fno-builtin-vsnprintf
    -fno-builtin-printf
    -fno-builtin-sprintf
    -fno-builtin-snprintf)

target_link_options(${EXECUTABLE} PRIVATE
    -T${MCU_LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    -Wl,--wrap=vprintf
    -Wl,--wrap=vsnprintf
    -Wl,--wrap=printf
    -Wl,--wrap=sprintf
    -Wl,--wrap=snprintf
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -Wl,--print-memory-usage)

#---------------------------------------------------------------------------------------
# Option
#---------------------------------------------------------------------------------------
# Allow switching of log interface
set(LOG_INTERFACE_LIST none uart uart_dma rtt)
set(LOG_INTERFACE uart CACHE STRING "Default log interface is uart")
set_property(CACHE LOG_INTERFACE PROPERTY STRINGS ${LOG_INTERFACE_LIST})
set_property(CACHE LOG_INTERFACE PROPERTY ADVANCED)

if(${LOG_INTERFACE} STREQUAL none)
    message(STATUS "Log interface use none")
    target_compile_definitions(${EXECUTABLE} PRIVATE
        LOG_INTERFACE_NONE
    )
elseif(${LOG_INTERFACE} STREQUAL uart)
    message(STATUS "Log interface use UART")
    target_compile_definitions(${EXECUTABLE} PRIVATE
        LOG_INTERFACE_USE_LPUART
    )
    target_sources(${EXECUTABLE} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Log/tiny_printf/tiny_printf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Log/hal_lpuart.c
    )
    target_include_directories(${EXECUTABLE} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Log/tiny_printf
    )
elseif(${LOG_INTERFACE} STREQUAL uart_dma)
    message(STATUS "Log interface use UART with DMA")
    target_compile_definitions(${EXECUTABLE} PRIVATE
        LOG_INTERFACE_USE_LPUART
        LOG_LPUART_USE_DMA
    )
    target_sources(${EXECUTABLE} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Log/tiny_printf/tiny_printf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Log/hal_lpuart.c
    )
    target_include_directories(${EXECUTABLE} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Log/tiny_printf
    )
elseif(${LOG_INTERFACE} STREQUAL rtt)
    message(STATUS "Log interface use RTT")
    target_compile_definitions(${EXECUTABLE} PRIVATE
        LOG_INTERFACE_USE_RTT
    )
    target_sources(${EXECUTABLE} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Log/segger_rtt/SEGGER_RTT.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Log/segger_rtt/SEGGER_RTT_printf.c
    )
    target_include_directories(${EXECUTABLE} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Log/segger_rtt
    )
else()
    message(FATAL_ERROR "Unsupported log interface selected...")
endif()

#---------------------------------------------------------------------------------------
# Build type
#---------------------------------------------------------------------------------------
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

if (WIN32)
    add_custom_command(TARGET ${EXECUTABLE}
      POST_BUILD
      COMMAND powershell -Command "Get-Date; Write-Output 'Build completed'")
else ()
    add_custom_command(TARGET ${EXECUTABLE}
      POST_BUILD
      COMMAND bash -c "date +'%x %T' && echo 'Build completed'")
endif ()
