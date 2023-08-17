cmake_minimum_required(VERSION 3.4)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "-stdlib=libc++")

project(AttacusAll LANGUAGES CXX)

include(./cmake/Config.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /O2 /bigobj")
#set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#
# GLM
#
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "Build static library" FORCE)

add_subdirectory(depot/glm)

#
# pybind11
#
if(${HAS_PYBIND11})
set(-DCMAKE_CXX_STANDARD=20 -std=c++20)
set(PYBIND11_PYTHON_VERSION 3.10)    
find_package(Python COMPONENTS Interpreter Development)
add_subdirectory(depot/pybind11)
endif() #HAS_PYBIND11
#

#
# SDL - Minimal build for now
#
set(HAVE_LIBC ON)
set(SDL_TEST OFF CACHE BOOL "Build the SDL2_test library" FORCE)
set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library" FORCE)
set(SDL_STATIC_PIC ON CACHE BOOL "Static version of the library should be built with Position Independent Code" FORCE)

#set(SDL_RENDER OFF CACHE BOOL "Use render subsystem" FORCE)
set(SDL_AUDIO OFF CACHE BOOL "Use audio subsystem" FORCE)
set(SDL_HAPTIC OFF CACHE BOOL "Use haptic subsystem" FORCE)
set(HIDAPI OFF CACHE BOOL "Use HIDAPI for low level joystick drivers" FORCE)

set(SDL_X11 ${ATT_WM_X11} CACHE BOOL "Enable X11 support" FORCE)
set(SDL_WAYLAND ${ATT_WM_WAYLAND} CACHE BOOL "Enable Wayland support" FORCE)
set(SDL_KMSDRM ${ATT_WM_DRM} CACHE BOOL "Enable DRM/KMS support" FORCE)
set(SDL_LIBUDEV ${ATT_WM_DRM} CACHE BOOL "Enable libudev support" FORCE)

set(UDEV_LIB_SONAME libudev.so)

if(${HAS_SDL})
add_subdirectory(depot/sdl)
endif() #HAS_SDL

#
# Glad
#
if(${ATT_RENDERER_GL})
    if(${HAS_GLAD})
        add_subdirectory(src/glad)
    endif()
endif()

#
#Attacus
#
add_subdirectory(src/attacus)

#
#AttacusPy
#
if(${HAS_PY})
add_subdirectory(src/attacus_py)
endif() #HAS_PY

#
#Examples
#
if(${HAS_EXAMPLES})
add_subdirectory(examples)
endif() #HAS_EXAMPLES

#
# spdlog
#
add_subdirectory(depot/spdlog)
set_target_properties(spdlog PROPERTIES POSITION_INDEPENDENT_CODE ${ATT_STATIC_PIC})

#
# googletest
#
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
#add_subdirectory(depot/googletest)

#
#Tests
#
#add_subdirectory(tests)