include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Standard.cmake)

function(USES_FLUTTER_ENGINE THIS)
    USES_STD(${THIS})
    target_include_directories(${THIS} PRIVATE
        ${FLUTTER_ENGINE}
    )
    target_precompile_headers(${THIS} PRIVATE ${FLUTTER_ENGINE}/flutter_embedder.h)
endfunction()