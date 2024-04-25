option(DUMP_HEX "Create intel hex file of final executable" ON)
option(DUMP_BIN "Create binary file of final executable" ON)
option(DUMP_ASM "Extract executeable assembly of final executable" ON)
option(DUMP_NM "Extract symbol of final executable" ON)

function(generateExtraOutput TARGET_NAME)
    if (${DUMP_HEX})
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            DEPENDS ${TARGET_NAME}
            COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.hex)
    endif()
    if (${DUMP_BIN})
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            DEPENDS ${TARGET_NAME}
            COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.bin)
    endif()
    if (${DUMP_ASM})
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_OBJDUMP} -S $<TARGET_FILE:${TARGET_NAME}> > ${TARGET_NAME}.asm)
    endif()
    if (${DUMP_NM})
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_NM} -C -l -n $<TARGET_FILE:${TARGET_NAME}> > ${TARGET_NAME}.sym)
    endif()
endfunction()