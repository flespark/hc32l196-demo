set(CORE_LIB_NAME mcu_core)

project(${CORE_LIB_NAME})

add_library(${CORE_LIB_NAME} STATIC)

# Accessible log interface
set(LOG_INTERFACE_LIST none uart uart_dma rtt)
set(LOG_INTERFACE uart CACHE STRING "Default log interface is uart")
set_property(CACHE LOG_INTERFACE PROPERTY STRINGS ${LOG_INTERFACE_LIST})
set_property(CACHE LOG_INTERFACE PROPERTY ADVANCED)

if(${LOG_INTERFACE} STREQUAL none)
    message(STATUS "Log not enabled")
elseif(${LOG_INTERFACE} STREQUAL uart)
    message(STATUS "Log interface use UART")
    target_compile_definitions(${CORE_LIB_NAME} PUBLIC
        LOG_USING_LPUART
    )
elseif(${LOG_INTERFACE} STREQUAL uart_dma)
    message(STATUS "Log interface use UART with DMA")
    target_compile_definitions(${CORE_LIB_NAME} PUBLIC
        LOG_USING_LPUART_DMA
    )
elseif(${LOG_INTERFACE} STREQUAL rtt)
    message(STATUS "Log interface use RTT, NOTICE: RTT will wake MCU from low power")
    target_compile_definitions(${CORE_LIB_NAME} PUBLIC
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

target_sources(${CORE_LIB_NAME} PRIVATE ${CORE_SOURCES})

target_include_directories(${CORE_LIB_NAME} PUBLIC
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

install(TARGETS   ${CORE_LIB_NAME}
    EXPORT        ${CORE_LIB_NAME}Targets
    ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CORE_LIB_NAME}
    COMPONENT     library
)

install(EXPORT ${CORE_LIB_NAME}Targets
    FILE ${CORE_LIB_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${CORE_LIB_NAME}
)
