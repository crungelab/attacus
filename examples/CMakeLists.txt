cmake_minimum_required(VERSION 3.30...4.0 FATAL_ERROR)

project(AttExamples LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(${ATT_ROOT}/cmake/Attacus.cmake)

set(HEADERS
    example_app.h
)

set(SOURCES
    example_app.cpp
)

add_library(${THIS} STATIC ${HEADERS} ${SOURCES})
USES_ATTACUS(${THIS})

add_subdirectory(basic)
add_subdirectory(methodchannel)
