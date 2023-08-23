include_guard()

# Standard includes
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CMakeDependentOption)

set(ATT_STATIC_PIC ON)
#
# Directories
#
set(ATT_ROOT ${CMAKE_SOURCE_DIR})

set(SPDLOG_ROOT ${ATT_ROOT}/depot/spdlog)
set(PYBIND11_ROOT ${ATT_ROOT}/depot/pybind11)
set(GLM_ROOT ${ATT_ROOT}/depot/glm)
set(SDL_ROOT ${ATT_ROOT}/depot/sdl)
set(GLAD_ROOT ${ATT_ROOT}/src/glad)

if(DEFINED ENV{FLUTTER_ENGINE})
    #set(ENGINE_OUT $ENV{FLUTTER_ENGINE}/out/host_debug_unopt)
    set(ENGINE_OUT $ENV{FLUTTER_ENGINE}/out/$ENV{LOCAL_ENGINE})
  else()
    set(ENGINE_OUT ${ATT_ROOT}/depot/flutter-engine)
endif()

set(EXAMPLES_ROOT ${ATT_ROOT}/examples)

set(GOOGLE_TEST_ROOT ${ATT_ROOT}/depot/googletest)

#
# Platform
#
set(ATT_PLATFORM ${CMAKE_SYSTEM_NAME})

set(ATT_PLATFORM_WINDOWS OFF)
set(ATT_PLATFORM_LINUX OFF)

if(${ATT_PLATFORM} STREQUAL "Windows")
    set(ATT_PLATFORM_WINDOWS ON)
elseif(${ATT_PLATFORM} STREQUAL "Linux")
    set(ATT_PLATFORM_LINUX ON)
endif()

#
# Renderer
#

#option(ATT_RENDERER_GL "Use GL" OFF)
#cmake_dependent_option(ATT_RENDERER_VULKAN "Use Vulkan" ON
#                       "ATT_PLATFORM_LINUX; NOT ATT_RENDERER_GL" OFF)

set(ATT_RENDERER_GL ON)
set(ATT_RENDERER_VULKAN OFF)

option(ATT_WM_WAYLAND "Use Wayland" OFF)
option(ATT_WM_DRM "Use DRM/KMS" OFF)
option(ATT_EMBEDDED "Embedded Device" OFF)

cmake_dependent_option(ATT_WM_X11 "Use X11" ON "ATT_PLATFORM_LINUX; NOT ATT_WM_WAYLAND; NOT ATT_WM_DRM" OFF)



set(ATT_COMPILE_DEFS 
    NOMINMAX=1
)

if(${ATT_WM_X11})
    message("Using X11")
    set(ATT_COMPILE_DEFS ${ATT_COMPILE_DEFS}
        SDL_VIDEO_DRIVER_X11=1
    )
elseif(${ATT_WM_WAYLAND})
    message("Using Wayland")
    set(ATT_COMPILE_DEFS ${ATT_COMPILE_DEFS}
        SDL_VIDEO_DRIVER_WAYLAND=1
    )
    if(${ATT_EMBEDDED})
        set(ATT_COMPILE_DEFS ${ATT_COMPILE_DEFS}
            SDL_DISABLE_SYSWM_X11=1
            SDL_DISABLE_SYSWM_X11_TYPES=1
            SDL_VIDEO_DRIVER_WAYLAND_QT_TOUCH=1
        )
    endif()
elseif(${ATT_WM_DRM})
    message("Using DRM/KMS")
    set(ATT_COMPILE_DEFS ${ATT_COMPILE_DEFS}
        SDL_VIDEO_DRIVER_KMSDRM=1
        SDL_DISABLE_SYSWM_X11=1
        SDL_DISABLE_SYSWM_X11_TYPES=1
    )
endif()

# TODO: Needs be CMake options
# Comment out items to minimize builds for testing
set(HAS_APP 1)
set(HAS_SHELL 1)
set(HAS_FLUTTER 1)
#set(HAS_PYBIND11 1)
set(HAS_SDL 1)
set(HAS_GLAD 1)
#set(HAS_PY 1)
set(HAS_EXAMPLES 1)
