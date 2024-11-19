cmake_minimum_required(VERSION 3.22)
###############################################################################
include(cmake/gcc-arm-none-eabi.cmake)
include(cmake/binutils.cmake)
include(cmake/gdb-helper.cmake)

if ("${PROJECT_NAME}" STREQUAL "")
    set(PROJECT_NAME firmware)
endif()
project(${PROJECT_NAME})

# Set build target
if(NOT DEFINED CMAKE_BUILD_TARGET)
    set(CMAKE_BUILD_TARGET ${CMAKE_PROJECT_NAME})
endif()
message("Build target: ${CMAKE_BUILD_TARGET}")

# Set debug mode as default
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()
message("Build type: ${CMAKE_BUILD_TYPE}")

enable_language(C ASM)

###############################################################################

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Device/HC32L196PC_GCC.ld)
set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Core/cmsis/Device/HDSC/HC32L19X/Source/GCC/startup_hc32l19x.s)

include(cmake/hc32l19x.cmake)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Core)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Drivers/xxx)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Test)
###############################################################################
add_executable(${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/App/src/*.c
)

set(PROJECT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/App/inc
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCES}
)

# used to export hardware relativ macro define
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${PROJECT_INCLUDE_DIRECTORIES}
)

target_link_libraries(${CMAKE_PROJECT_NAME} mcu_core)

generate_debugger_config(${CMAKE_BUILD_TARGET})
###############################################################################
generate_extra_output(${CMAKE_PROJECT_NAME})

if (WIN32)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND powershell -Command "Get-Date; Write-Output 'Build completed'")
else ()
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND bash -c "date +'%x %T' && echo 'Build completed'")
endif ()