include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Attacus.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Shell.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Flutter.cmake)

function(USES_EXAMPLES THIS)
    USES_ATTACUS(${THIS})
    USES_SHELL(${THIS})
    USES_FLUTTER(${THIS})

    target_include_directories(${THIS} PRIVATE
        ${ATT_ROOT}
    )

    target_link_libraries(${THIS} PRIVATE AttExamples)

    #TODO: This makes debugging easier
    set_target_properties(${THIS} PROPERTIES
        BUILD_RPATH "$ORIGIN"
        BUILD_RPATH_USE_ORIGIN TRUE
    )

    set_target_properties(
        ${THIS} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${ATT_ROOT}/examples/${THIS}"
    )

    set_target_properties(
        ${THIS} PROPERTIES
        FOLDER "Examples"
    )

    if(${ATT_PLATFORM_WINDOWS})
        set(ENGINE_PATH "${ENGINE_OUT}/flutter_engine.dll")
    elseif(${ATT_PLATFORM_LINUX})
        set(ENGINE_PATH "${ENGINE_OUT}/libflutter_engine.so")
    endif()

    add_custom_command(TARGET ${THIS} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ENGINE_PATH}
        $<TARGET_FILE_DIR:${THIS}>
    )

    #add_custom_command(TARGET ${THIS} POST_BUILD
    #    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${THIS}> $<TARGET_FILE_DIR:${THIS}>
    #    COMMAND_EXPAND_LISTS
    #)

endfunction()

function(add_example THIS)
    add_executable(${THIS} ${ARGN})
    USES_EXAMPLES(${THIS})
endfunction()