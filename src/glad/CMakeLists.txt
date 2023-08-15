cmake_minimum_required(VERSION 3.4)

set(THIS Glad)

project(${THIS} LANGUAGES C CXX)

set(_ ${CMAKE_CURRENT_SOURCE_DIR}
)

set(GLAD_HEADERS
    ${_}/include/glad/glad.h
    ${_}/include/KHR/khrplatform.h
)

set(GLAD_SRC
    ${_}/src/glad.c
)

add_library(${THIS} STATIC ${GLAD_SRC} ${GLAD_HEADERS})
set_target_properties(${THIS} PROPERTIES POSITION_INDEPENDENT_CODE ${ATT_STATIC_PIC})

target_include_directories(${THIS} PRIVATE
    ${_}/include
)