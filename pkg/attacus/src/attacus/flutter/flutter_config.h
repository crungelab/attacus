#pragma once

#include <string>

namespace attacus {

class FlutterConfig {
public:
    FlutterConfig();
    std::string assets_path_;
    std::string icu_data_path_;
};

} // namespace attacus