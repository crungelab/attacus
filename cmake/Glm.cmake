include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Config.cmake)

function(USES_GLM THIS)
  target_include_directories(${THIS} PRIVATE
    ${GLM_ROOT}
  )
  target_link_libraries(${THIS} PRIVATE glm)

endfunction()
