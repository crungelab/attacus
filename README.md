# Attacus :snake: :butterfly:

Flutter Python Extension

[Flutter](https://flutter.dev/)

[Flutter Embedder](https://github.com/flutter/flutter/wiki/Custom-Flutter-Engine-Embedders)

[SDL](https://github.com/libsdl-org/SDL/)

## Development

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
