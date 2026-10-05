cmake_minimum_required(VERSION 3.30...4.0 FATAL_ERROR)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "-stdlib=libc++")
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

project(AttacusAll LANGUAGES CXX)

include(./cmake/Config.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#
# GLM
#
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "Build static library" FORCE)

add_subdirectory(depot/json)

add_subdirectory(depot/glm)

#
# Python / pybind11
#
set(PYBIND11_FINDPYTHON ON)
find_package(Python 3.12 REQUIRED COMPONENTS Interpreter Development.Module Development.Embed)
add_subdirectory(depot/pybind11 EXCLUDE_FROM_ALL)


#
# SDL - Minimal build for now
#
set(HAVE_LIBC ON)
set(SDL_TEST OFF CACHE BOOL "Build the SDL3_test library" FORCE)
set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library" FORCE)
set(SDL_STATIC_PIC ON CACHE BOOL "Static version of the library should be built with Position Independent Code" FORCE)

set(SDL_RENDER OFF CACHE BOOL "Use render subsystem" FORCE)
set(SDL_AUDIO OFF CACHE BOOL "Use audio subsystem" FORCE)
set(SDL_HAPTIC OFF CACHE BOOL "Use haptic subsystem" FORCE)
set(HIDAPI OFF CACHE BOOL "Use HIDAPI for low level joystick drivers" FORCE)

set(VIDEO_X11 ${ATT_WM_X11} CACHE BOOL "Enable X11 support" FORCE)
set(VIDEO_WAYLAND ${ATT_WM_WAYLAND} CACHE BOOL "Enable Wayland support" FORCE)

if(${HAS_SDL})
add_subdirectory(depot/sdl)
endif() #HAS_SDL

#
#Attacus
#
add_subdirectory(src)
add_subdirectory(tests)

#
#Examples
#
if(${HAS_EXAMPLES})
add_subdirectory(examples)
endif() #HAS_EXAMPLES

#
# googletest
#
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
add_subdirectory(depot/googletest)

#
# spdlog
#
add_subdirectory(depot/spdlog)
set_target_properties(spdlog PROPERTIES POSITION_INDEPENDENT_CODE ${ATT_STATIC_PIC})
