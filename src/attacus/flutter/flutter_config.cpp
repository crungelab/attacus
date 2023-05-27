#include <filesystem>
namespace fs = std::filesystem;

#include "flutter_config.h"

namespace attacus {
    FlutterConfig::FlutterConfig() {
        fs::path project_path = fs::current_path();
        fs::path assets_path = project_path / "build" / "flutter_assets";
        fs::path icu_data_path = project_path / "build" / "icudtl.dat";
        assets_path_ = assets_path.string();
        icu_data_path_ = icu_data_path.string();
    }
}