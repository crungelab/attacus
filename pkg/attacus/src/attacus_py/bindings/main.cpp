#include <limits>
//#include <filesystem>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <spdlog/spdlog.h>

#include "bindtools.h"

#include <attacus/app.h>
#include <attacus/flutter/flutter_view.h>
#include <attacus/flutter/flutter_messenger.h>
#include <attacus/flutter/standard_method_channel.h>

namespace py = pybind11;

using namespace attacus;

// The variant types are mapped with Dart types in following ways:
// std::monostate       -> null
// bool                 -> bool
// int32_t              -> int
// int64_t              -> int
// double               -> double
// std::string          -> String
// std::vector<uint8_t> -> Uint8List
// std::vector<int32_t> -> Int32List
// std::vector<int64_t> -> Int64List
// std::vector<float>   -> Float32List
// std::vector<double>  -> Float64List
// EncodableList        -> List
// EncodableMap         -> Map

template<class T>
struct always_false : std::false_type {};

EncodableValue encode(py::object obj) {
    PyObject* object = obj.ptr();
    EncodableValue value;
    if (Py_IsNone(object)) {
        value = nullptr;
    }
    else if (PyLong_Check(object)) {
        int64_t val = PyLong_AsLong(object);
        value = val;
    }
    else if (PyUnicode_Check(object)) {
        auto val = PyUnicode_AsUTF8(object);
        value = val;
    }
    else if (PyBytes_Check(object)) {
        // Get size and pointer to data
        Py_ssize_t size = PyBytes_Size(object);
        char* data = PyBytes_AsString(object);

        // Convert to std::vector<uint8_t>
        std::vector<uint8_t> result(data, data + size);
        value = result;
    }
    return value;
}

class PyApp : public App {
public:
    /* Inherit the constructors */
    using App::App;
    PyApp() : App() {}
    /* Trampoline (need one for each virtual function) */
    void Loop() override {
        PYMETHOD_OVERRIDE(
            void,       // Return type
            App,        // Parent class
            loop,    // Python function name
            Loop     // C++ function name
        );
    }
    void Startup() override {
        PYMETHOD_OVERRIDE(
            void,       // Return type
            App,        // Parent class
            startup,    // Python function name
            Startup     // C++ function name
        );
    }
    void Shutdown() override {
        PYMETHOD_OVERRIDE(
            void,
            App,
            shutdown,
            Shutdown
        );
    }
};

class PyFlutterView : public FlutterView {
public:
    /* Inherit the constructors */
    using FlutterView::FlutterView;
    PyFlutterView(View& parent, FlutterConfig& config, ViewParams params = ViewParams()) : FlutterView(parent, config) {}
    /* Trampoline (need one for each virtual function) */
    void Startup() override {
        PYMETHOD_OVERRIDE(
            void,           // Return type
            FlutterView,    // Parent class
            startup,        // Python function name
            Startup         // C++ function name
        );
    }
    void Shutdown() override {
        PYMETHOD_OVERRIDE(
            void,
            FlutterView,
            shutdown,
            Shutdown
        );
    }
};

void init_main(py::module &attacus, Registry &registry) {

    PYCLASS_O_BEGIN(attacus, App, PyApp)
        .def(py::init<>([](){
            return App::Produce<PyApp>();
        }))
        .def("run", [](App& self) {
            self.Run();
        })
        .def("process_events", &App::ProcessEvents)
        .def("startup", &App::Startup)
        .def("shutdown", &App::Shutdown)
    PYCLASS_END(attacus, App)

    PYCLASS_BEGIN(attacus, FlutterConfig)
        .def(py::init<>())
        .def_readwrite("assets_path", &FlutterConfig::assets_path_)
        .def_readwrite("icu_data_path", &FlutterConfig::icu_data_path_)
    PYCLASS_END(attacus, FlutterConfig)

    //PYCLASS_BEGIN(attacus, FlutterView)
    PYCLASS_O_BEGIN(attacus, FlutterView, PyFlutterView)
        .def(py::init<>([](App& parent, FlutterConfig& config){
            return FlutterView::Produce<PyFlutterView>(parent, config);
        }))
        .def("startup", &FlutterView::Startup)
        .def("shutdown", &FlutterView::Shutdown)
        .def_readonly("messenger", &FlutterView::messenger_)
        //.def_readwrite("assets_path", &FlutterView::assets_path_)
        //.def_readwrite("icu_data_path", &FlutterView::icu_data_path_)
    PYCLASS_END(attacus, FlutterView)

    PYCLASS_BEGIN(attacus, FlutterMessenger)
    PYCLASS_END(attacus, FlutterMessenger)

    //PYCLASS_BEGIN(attacus, StandardMethod)
    auto _StandardMethod = py::class_<StandardMethod, std::unique_ptr<StandardMethod, py::nodelete>>(attacus, "StandardMethod")
        .def(py::init<>([](StandardMethodChannel& channel, const std::string& name, py::object cb){
            std::cout << "StandardMethod: " << name << std::endl;
            std::cout << cb << std::endl;
            return new StandardMethod(
                channel,
                name,
                [cb](const StandardMethodCall &call, std::unique_ptr<StandardMethodResult> result){
                    std::cout << "StandardMethodCall:" << std::endl;
                    std::cout << cb << std::endl;
                    auto py_call = py::cast(&call);
                    std::cout << "py_call: " << py_call << std::endl;
                    auto py_result = py::cast(std::move(result));
                    std::cout << "py_result: " << py_result << std::endl;
                    try {
                        py::object val = cb(py_call, py_result);
                    } catch (py::error_already_set &e) {
                        //std::cerr << "Caught Python exception: " << e.what() << std::endl;
                        spdlog::error("Python: {}", e.what());
                    }
                }
            );
        }))
    PYCLASS_END(attacus, StandardMethod)

    PYCLASS_BEGIN(attacus, StandardMethodCall)
        .def("method_name", &StandardMethodCall::method_name)
        .def("arguments", &StandardMethodCall::arguments)
    PYCLASS_END(attacus, StandardMethodCall)

    /*PYCLASS_BEGIN(attacus, StandardMethodResult)
    //auto _StandardMethodResult = py::class_<StandardMethodResult, std::unique_ptr<StandardMethodResult, py::nodelete>>(attacus, "StandardMethodResult")
        .def("success", [](StandardMethodResult& self, py::object obj) {
            PyObject* object = obj.ptr();
            if (Py_IsNone(object)) {
                return self.Success();
            }
            else if (PyLong_Check(object)) {
                int64_t val = PyLong_AsLong(object);
                return self.Success(EncodableValue(val));
            }
            else if (PyBytes_Check(object)) {
                // Get size and pointer to data
                Py_ssize_t size = PyBytes_Size(object);
                char* data = PyBytes_AsString(object);

                // Convert to std::vector<uint8_t>
                std::vector<uint8_t> val(data, data + size);
                return self.Success(EncodableValue(val));
            }

        }
        , py::arg("value") = nullptr
        )
    PYCLASS_END(attacus, StandardMethodResult)*/

    PYCLASS_BEGIN(attacus, StandardMethodResult)
    //auto _StandardMethodResult = py::class_<StandardMethodResult, std::unique_ptr<StandardMethodResult, py::nodelete>>(attacus, "StandardMethodResult")
        .def("success", [](StandardMethodResult& self, py::object obj) {
            PyObject* object = obj.ptr();
            if (Py_IsNone(object)) {
                return self.Success();
            }
            else {
                return self.Success(encode(obj));
            }
        }
        , py::arg("value") = nullptr
        )
    PYCLASS_END(attacus, StandardMethodResult)


    //PYCLASS_BEGIN(attacus, EncodableValue)
    auto _EncodableValue = py::class_<EncodableValue, std::unique_ptr<EncodableValue, py::nodelete>>(attacus, "EncodableValue")
        .def("decode", [](EncodableValue& self) {
                auto variant = self;
                return std::visit([](auto&& arg) -> py::object {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, int>) {
                        return py::int_(arg);
                    } else if constexpr (std::is_same_v<T, double>) {
                        return py::float_(arg);
                    } else if constexpr (std::is_same_v<T, std::string>) {
                        return py::str(arg);
                    } else if constexpr (std::is_same_v<T, std::vector<uint8_t>>) {
                        return py::bytes((char*)arg.data(), arg.size());
                    } else {
                        //static_assert(always_false<T>::value, "non-exhaustive visitor!");
                        return py::none();
                    }
                }, variant);
            })
    PYCLASS_END(attacus, EncodableValue)

    PYCLASS_BEGIN(attacus, StandardMethodChannel)
        .def(py::init<>([](FlutterMessenger& messenger, const std::string& name){
            std::cout << "StandardMethodChannel: " << name << std::endl;
            return new StandardMethodChannel(
                messenger,
                name
            );
        }))
        .def("invoke_method", [](StandardMethodChannel& self, const std::string &method, py::object obj) {
            EncodableValue value = encode(obj);
            std::unique_ptr<EncodableValue> args = std::make_unique<EncodableValue>(value);
            return self.InvokeMethod(method, std::move(args));
        }
            , py::arg("name")
            , py::arg("arguments")
            //, py::arg("result") = nullptr
            , py::return_value_policy::automatic_reference
        )
    PYCLASS_END(attacus, StandardMethodChannel)
   
}
