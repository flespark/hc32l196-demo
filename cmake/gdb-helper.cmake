#---------------------------------------------------------------------------------------
# Generates a Visual Studio Code launch configuration for debugging with JLink.
#---------------------------------------------------------------------------------------
function(generate_debugger_config TARGET_NAME)
    get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)
    if (NOT "${TARGET_TYPE}" STREQUAL "EXECUTABLE")
        message(STATUS "Skipping debugger config for non-executable target: ${TARGET_NAME}")
        return()
    endif()

    find_program(GDB_PATH NAMES arm-none-eabi-gdb)
    if(GDB_PATH)
        get_filename_component(GDB_DIR "${GDB_PATH}" DIRECTORY)
        set(TOOLCHAIN_FOLDER ${GDB_DIR})
        message("GDB found at: ${GDB_DIR}")
    elseif(DEFINED TOOLCHAIN_FOLDER)
        message(STATUS "Using user-provided TOOLCHAIN_FOLDER: ${TOOLCHAIN_FOLDER}")
        if(EXISTS "${TOOLCHAIN_FOLDER}/arm-none-eabi-gdb${CMAKE_EXECUTABLE_SUFFIX}")
            set(GDB_PATH "${TOOLCHAIN_FOLDER}/arm-none-eabi-gdb${CMAKE_EXECUTABLE_SUFFIX}")
        else()
            message(WARNING "arm-none-eabi-gdb not found in provided TOOLCHAIN_FOLDER: ${TOOLCHAIN_FOLDER}")
        endif()
    else()
        message(WARNING "GDB not found and TOOLCHAIN_FOLDER not provided.")
        message(STATUS "Please run CMake with -DTOOLCHAIN_FOLDER=/path/to/toolchain")
    endif()

    get_target_property(TARGET_BINARY_DIR ${TARGET_NAME} BINARY_DIR)
    if(NOT TARGET_BINARY_DIR)
        set(TARGET_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    set(TARGET_ELF ${TARGET_BINARY_DIR}/${TARGET_NAME}.elf)
    set(TARGET_BIN ${TARGET_BINARY_DIR}/${TARGET_NAME}.bin)

    set(LAUNCH_JSON_IN "${CMAKE_SOURCE_DIR}/cmake/launch.json.in")
    set(LAUNCH_JSON_OUT "${CMAKE_SOURCE_DIR}/.vscode/launch.json")
    configure_file(${LAUNCH_JSON_IN} ${LAUNCH_JSON_OUT} @ONLY)

    set(FLASH_JLINK_IN "${CMAKE_SOURCE_DIR}/cmake/flash_mcu.jlink.in")
    set(FLASH_JLINK_OUT "${CMAKE_SOURCE_DIR}/Device/flash_mcu.jlink")
    configure_file(${FLASH_JLINK_IN} ${FLASH_JLINK_OUT} @ONLY)
endfunction()