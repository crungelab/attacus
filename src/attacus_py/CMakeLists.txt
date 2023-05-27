cmake_minimum_required(VERSION 3.4)

set(THIS AttacusPy)
set(MODULE attacus)

project(${THIS} VERSION 0.1 LANGUAGES CXX)

include(${ATT_ROOT}/cmake/Pybind11.cmake)
include(${ATT_ROOT}/cmake/Attacus.cmake)

set(ATTPY_HEADERS
    bindings/bindtools.h
    bindings/conversions.h
)

set(ATTPY_SOURCES
    #bindings/generated.cpp
    bindings/main.cpp
)

pybind11_add_module(${THIS} MODULE
    bindings/init.cpp
)

set_target_properties(${THIS} PROPERTIES OUTPUT_NAME ${MODULE})

USES_PYBIND11(${THIS})
USES_ATTACUS(${THIS})

target_sources(${THIS} PRIVATE ${ATTPY_SOURCES})
# add include files
target_sources(${THIS} PRIVATE ${ATTPY_HEADERS}) # for intellisense
target_include_directories(${THIS}
	PUBLIC
		$<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS ${THIS} DESTINATION ${MODULE})

#set (FLUTTER_DLL ${ATT_ROOT}/depot/flutter-engine/flutter_engine.dll)
#install(FILES ${FLUTTER_DLL} DESTINATION ${MODULE})

# Get the FLUTTER_ROOT environment variable
#file(TO_CMAKE_PATH $ENV{FLUTTER_ROOT} FLUTTER_ROOT)

# Set the path to icudtl.dat
#set(ICU_DATA ${FLUTTER_ROOT}/bin/cache/artifacts/engine/windows-x64/icudtl.dat)

# Install icudtl.dat to the same directory as your .pyd file
#install(FILES ${ICU_DATA} DESTINATION ${MODULE})
