include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Attacus.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Shell.cmake)

function(USES_EXAMPLE_VIEWS THIS)
    USES_ATTACUS(${THIS})
    USES_SHELL(${THIS})

    target_include_directories(${THIS} PRIVATE
        ${ATT_ROOT}
    )

    target_link_libraries(${THIS} PRIVATE AttExamples)
    
    set_target_properties(
        ${THIS} PROPERTIES
        FOLDER "ExampleViews"
    )
        
endfunction()
