cmake_minimum_required(VERSION 3.4)

project(AttFlutter LANGUAGES CXX)

set(THIS ${PROJECT_NAME})

include(${ATT_ROOT}/cmake/Sdl.cmake)
include(${ATT_ROOT}/cmake/Shell.cmake)
include(${ATT_ROOT}/cmake/FlutterEngine.cmake)


set(FLUTTER_HEADERS
    flutter_config.h
    flutter_view.h
    flutter_messenger.h
    flutter_runner.h

    channel.h
    method_channel.h
    standard_method_channel.h
    standard_message_codec.h
    standard_method_codec.h
    standard_codec_serializer.h

    engine_method_result.h
    method_codec.h
    method_call.h
    method_result.h
    encodable_value.h
    byte_streams.h
    byte_buffer_streams.h
    message_codec.h
    json.h
    json_method_channel.h
    json_message_codec.h
    json_method_codec.h

    components/flutter_component.h
    components/cursor.h
    components/mouse_input.h
    components/text_input.h
    components/text_input_model.h
    components/text_range.h
    components/string_conversion.h
    components/texture_registrar.h
    components/view_registry.h

    compositor/compositor.h
    compositor/gl/backing_surface_gl.h
    compositor/gl/compositor_gl.h

)

set(FLUTTER_SRC
    flutter_config.cpp
    flutter_view.cpp
    flutter_messenger.cpp
    flutter_runner.cpp

    standard_codec.cpp
    json_message_codec.cpp
    json_method_codec.cpp

    components/flutter_component.cpp
    components/cursor.cpp
    components/mouse_input.cpp
    components/text_input.cpp
    components/text_input_model.cpp
    components/string_conversion.cpp
    components/texture_registrar.cpp
    components/view_registry.cpp

    compositor/compositor.cpp
    compositor/gl/backing_surface_gl.cpp
    compositor/gl/compositor_gl.cpp

)

add_library(${THIS} STATIC ${FLUTTER_SRC} ${FLUTTER_HEADERS})
set_target_properties(${THIS} PROPERTIES POSITION_INDEPENDENT_CODE ${ATT_STATIC_PIC})

USES_SDL(${THIS})
USES_FLUTTER_ENGINE(${THIS})

if(${ATT_PLATFORM_WINDOWS})
add_library(FlutterEngine SHARED IMPORTED)
set_property(TARGET FlutterEngine PROPERTY
             IMPORTED_LOCATION ${ENGINE_OUT}/flutter_engine.dll)
set_property(TARGET FlutterEngine PROPERTY
             IMPORTED_IMPLIB ${ENGINE_OUT}/flutter_engine.dll.lib)
endif()

if(${ATT_PLATFORM_LINUX})
    add_library(FlutterEngine SHARED IMPORTED)
    set_property(TARGET FlutterEngine PROPERTY
                 IMPORTED_LOCATION ${ENGINE_OUT}/libflutter_engine.so)
endif()
    
target_include_directories(${THIS} PRIVATE
    ${ENGINE_OUT}
)

target_link_libraries(${THIS} PRIVATE FlutterEngine)
