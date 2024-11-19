set(CORE_INTERFACE mcu_core)

project(${CORE_INTERFACE})

add_library(${CORE_INTERFACE} INTERFACE)

# Accessible log interface
set(LOG_INTERFACE_LIST none uart uart_dma rtt)
set(LOG_INTERFACE uart CACHE STRING "Default log interface is uart")
set_property(CACHE LOG_INTERFACE PROPERTY STRINGS ${LOG_INTERFACE_LIST})
set_property(CACHE LOG_INTERFACE PROPERTY ADVANCED)

if(${LOG_INTERFACE} STREQUAL none)
    message(STATUS "Log not enabled")
elseif(${LOG_INTERFACE} STREQUAL uart)
    message(STATUS "Log interface use UART")
    target_compile_definitions(${CORE_INTERFACE} INTERFACE
        LOG_USING_LPUART
    )
elseif(${LOG_INTERFACE} STREQUAL uart_dma)
    message(STATUS "Log interface use UART with DMA")
    target_compile_definitions(${CORE_INTERFACE} INTERFACE
        LOG_USING_LPUART_DMA
    )
elseif(${LOG_INTERFACE} STREQUAL rtt)
    message(STATUS "Log interface use RTT, NOTICE: RTT will wake MCU from low power")
    target_compile_definitions(${CORE_INTERFACE} INTERFACE
        LOG_USING_RTT
    )
else()
    message(FATAL_ERROR "Unsupported log interface selected...")
endif()


file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    ${STARTUP_SCRIPT}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis/Device/HDSC/${MCU_FAMILY}/Source/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mcu/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hal/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/components/tiny_printf/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/components/segger_rtt/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/components/cm_backtrace/*.c
)

target_sources(${CORE_INTERFACE} INTERFACE ${CORE_SOURCES})

target_include_directories(${CORE_INTERFACE} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cmsis/Include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cmsis/Device/HDSC/${MCU_FAMILY}/Include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/mcu/inc>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/hal/inc>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components/tiny_printf>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components/segger_rtt>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components/cm_backtrace>
)