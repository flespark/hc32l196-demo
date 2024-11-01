option(DUMP_HEX "Create intel hex file of final executable" OFF)
option(DUMP_BIN "Create binary file of final executable" ON)
option(DUMP_ASM "Extract executeable assembly of final executable" OFF)
option(DUMP_NM "Extract symbol of final executable" OFF)

function(generate_extra_output EXECUTABLE_NAME)
    if (${DUMP_HEX})
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            DEPENDS ${EXECUTABLE_NAME}
            COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE_NAME}> $<TARGET_NAME:${EXECUTABLE_NAME}>.hex)
    endif()
    if (${DUMP_BIN})
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            DEPENDS ${EXECUTABLE_NAME}
            COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE_NAME}> $<TARGET_NAME:${EXECUTABLE_NAME}>.bin)
    endif()
    if (${DUMP_ASM})
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            COMMAND ${CMAKE_OBJDUMP} -S $<TARGET_FILE:${EXECUTABLE_NAME}> > $<TARGET_NAME:${EXECUTABLE_NAME}>.asm)
    endif()
    if (${DUMP_NM})
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            COMMAND ${CMAKE_NM} -C -l -n $<TARGET_FILE:${EXECUTABLE_NAME}> > $<TARGET_NAME:${EXECUTABLE_NAME}>.sym)
    endif()
endfunction()
