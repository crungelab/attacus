# Attacus :snake: :butterfly:

Python Flutter Extension

[Flutter](https://flutter.dev/)

[Flutter Embedder](https://github.com/flutter/flutter/wiki/Custom-Flutter-Engine-Embedders)

[SDL](https://github.com/libsdl-org/SDL/)

## Development

## Quick Start

    git clone --recursive https://github.com/kfields/fulkan
    cd fulkan
    mkdir build
    cd build
    cmake ..

## Debug

    mkdir build-debug
    cd build-debug
    cmake -DCMAKE_BUILD_TYPE=Debug ..

## Windows Debug

    mkdir build-debug
    cd build-debug
    cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Debug ..

## Wayland
    cmake -DATT_WM_WAYLAND=ON -DCMAKE_BUILD_TYPE=Debug ..

## DRM/KMS
    cmake -DATT_WM_DRM=ON -DCMAKE_BUILD_TYPE=Debug ..

## Cross Compiling
cmake -DATT_EMBEDDED=ON -DATT_WM_WAYLAND=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=$R5_CMAKE_TOOLCHAIN_FILE ..

cmake -DATT_EMBEDDED=ON -DATT_WM_DRM=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=$R5_CMAKE_TOOLCHAIN_FILE ..

cmake -DATT_EMBEDDED=ON -DATT_WM_DRM=ON -DCMAKE_BUILD_TYPE=Debug ..

cmake -DATT_EMBEDDED=ON -DATT_WM_WAYLAND=ON -DCMAKE_BUILD_TYPE=Debug ..

## Python

### Required

* [Hatch](https://hatch.pypa.io/)

### Optional

* [Ninja](https://ninja-build.org/)

### Clone

```bash
git clone https://github.com/crungelab/attacus
cd attacus
```

### Tool Chain

[cxbuild](https://github.com/crungelab/cxbuild)

[pybind11](https://github.com/pybind/pybind11)

### Develop
```bash
hatch shell
procure
cxbuild develop
```

### Build
```bash
cxbuild
```

### Visual Studio Code
```bash
hatch shell
code --reuse-window .
```

### [Flutter Custom Devices](https://github.com/flutter/flutter/wiki/Using-custom-embedders-with-the-Flutter-CLI)
This will enable and display the location of your .flutter_custom_devices.json file
```
flutter config --enable-custom-devices
flutter custom-devices
```
Add this to your .flutter_custom_devices.json file

``` json
    {
      "id": "attacus",
      "label": "Attacus",
      "sdkNameAndVersion": "Attacus 0.1",
      "platform": null,
      "enabled": true,
      "ping": [
        "ping",
        "-n",
        "1",
        "-w",
        "500",
        "localhost"
      ],
      "pingSuccessRegex": "[<=]\\d+ms",
      "postBuild": [
        "python",
        "-m",
        "attacus.post_build"
      ],
      "install": [
        "python",
        "-m",
        "attacus.install"
      ],
      "uninstall": [
        "python",
        "-m",
        "attacus.uninstall"
      ],
      "runDebug": [
        "python",
        "main.py"
      ],
      "forwardPort": null,
      "forwardPortSuccessRegex": null,
      "screenshot": null
    }
```

## Examples
```bash
flutter build bundle
python main.py
```
or
```bash
flutter run
```

## Flutter Engine
ln -s linux_debug_unopt_riscv64 host_debug_unopt_riscv64

flutter build bundle --local-engine=linux_debug_unopt_riscv64

./flutter/tools/gn --runtime-mode=debug --unoptimized --no-stripped --no-goma
ninja -C out/host_debug_unopt

