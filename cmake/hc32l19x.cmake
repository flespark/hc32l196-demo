#---------------------------------------------------------------------------------------
# Set startup script
#---------------------------------------------------------------------------------------
if(NOT DEFINED STARTUP_SCRIPT)
message(FATAL_ERROR "No startup script defined")
endif(NOT DEFINED STARTUP_SCRIPT)
message("Startup script: ${STARTUP_SCRIPT}")

#---------------------------------------------------------------------------------------
# Set linker script
#---------------------------------------------------------------------------------------
if(NOT DEFINED LINKER_SCRIPT)
message(FATAL_ERROR "No linker script defined")
endif(NOT DEFINED LINKER_SCRIPT)
message("Linker script: ${LINKER_SCRIPT}")


#---------------------------------------------------------------------------------------
# Set compiler/linker flags
#---------------------------------------------------------------------------------------

# Set export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set microcontroller information
set(TARGET_PROCESSOR cortex-m0plus)
set(MCU_FAMILY HC32L19X)
set(MCU_MODEL HC32L19xPxxx)

message("MCU model: ${MCU_MODEL}")

set(CPU_PARAMETERS
    -mcpu=${TARGET_PROCESSOR}
    -mthumb
    -mfloat-abi=soft
)

# Set C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Set MCU macros
add_compile_definitions(
    ${MCU_FAMILY}
    ${MCU_MODEL}
)

# Object build options
add_compile_options(
    ${CPU_PARAMETERS}
    -ffile-prefix-map=${CMAKE_SOURCE_DIR}=.
    -mabi=aapcs
    -Wall
    -Wextra
    # -Wpedantic
    -Wshadow
    -Wdouble-promotion
    -fno-unroll-loops
    -Wformat=2 -Wformat-truncation
    -Wundef
    -Wfatal-errors
    -Wno-unused-parameter
    -fno-common
    -ffreestanding
    -fno-builtin
    # -fsingle-precision-constant
    -fstack-usage
    -fno-builtin-vprintf
    -fno-builtin-vsnprintf
    -fno-builtin-printf
    -fno-builtin-sprintf
    -fno-builtin-snprintf
)

# Optimization flags
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
    add_compile_definitions(DEBUG)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os -flto)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-O0 -ggdb3)
    add_compile_definitions(DEBUG)
endif ()

# Linker flags
add_link_options(
    ${CPU_PARAMETERS}
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_BUILD_TARGET}.map
    -Wl,--print-memory-usage
    # -nostdlib
    -Wl,--wrap=vprintf
    -Wl,--wrap=vsnprintf
    -Wl,--wrap=printf
    -Wl,--wrap=sprintf
    -Wl,--wrap=snprintf
)