cmake_minimum_required(VERSION 3.4)

project(basic LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(${ATT_ROOT}/cmake/Examples.cmake)

set(HEADERS
)

set(SOURCES
    main.cpp
)

add_executable(${THIS} ${HEADERS} ${SOURCES})
USES_EXAMPLES(${THIS})
