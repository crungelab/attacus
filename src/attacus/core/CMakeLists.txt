cmake_minimum_required(VERSION 3.4)

project(AttCore LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

set(ATT_CORE_HEADERS
    component.h
    event.h
)

set(ATT_CORE_SOURCES
    component.cpp
)

add_library(${THIS} STATIC ${ATT_CORE_SOURCES} ${ATT_CORE_HEADERS})
set_target_properties(${THIS} PROPERTIES POSITION_INDEPENDENT_CODE ${ATT_STATIC_PIC})
