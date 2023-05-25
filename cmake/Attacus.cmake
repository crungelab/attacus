include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Shell.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Flutter.cmake)

function(USES_ATTACUS THIS)
    USES_SHELL(${THIS})
    USES_FLUTTER(${THIS})
    target_link_libraries(${THIS} PRIVATE Attacus)
endfunction()
