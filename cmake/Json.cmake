include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Config.cmake)

function(USES_JSON THIS)
  target_include_directories(${THIS} PRIVATE
    ${JSON_ROOT}/include
  )
  target_link_libraries(${THIS} PRIVATE nlohmann_json)

endfunction()
