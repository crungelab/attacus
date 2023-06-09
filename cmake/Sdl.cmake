cmake_minimum_required(VERSION 3.14)

include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Standard.cmake)

function(USES_SDL THIS)
    USES_STD(${THIS})
    target_include_directories(${THIS} PRIVATE
        #${SDL_ROOT}/include
        ${SDL_ROOT}/include/SDL3
    )
    target_precompile_headers(${THIS} PRIVATE ${SDL_ROOT}/include/SDL3/SDL.h)
    target_link_libraries(${THIS} PRIVATE SDL3-static)
endfunction()

