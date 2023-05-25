include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/FlutterEngine.cmake)

function(USES_FLUTTER THIS)
    USES_FLUTTER_ENGINE(${THIS})
    target_link_libraries(${THIS} PRIVATE AttFlutter)
endfunction()