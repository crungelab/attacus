import os
from procure import GitSolution, ZipSolution

FLUTTER_ROOT = os.environ.get("FLUTTER_ROOT")


def get_engine_version():
    with open(os.path.join(FLUTTER_ROOT, "bin", "internal", "engine.version"), "r") as f:
        return f.read().strip()


class GoogleTest(GitSolution):
    path = "depot/googletest"
    url = "https://github.com/google/googletest"


class Spdlog(GitSolution):
    path = "depot/spdlog"
    url = "https://github.com/gabime/spdlog"


class SDL(GitSolution):
    path = "depot/sdl"
    url = "https://github.com/libsdl-org/SDL"


class Pybind11(GitSolution):
    path = "depot/pybind11"
    url = "https://github.com/pybind/pybind11"


class GLM(GitSolution):
    path = "depot/glm"
    url = "https://github.com/g-truc/glm"


# TODO: Need to make this multi-platform
class FlutterEngine(ZipSolution):
    path = "depot/flutter-engine"
    # url = 'https://storage.googleapis.com/flutter_infra_release/flutter/54a7145303f0dd9d0f93424a2e124eb4abef5091/linux-x64/linux-x64-embedder.zip'
    engine_version = get_engine_version()
    print(engine_version)
    url = f"https://storage.googleapis.com/flutter_infra_release/flutter/{engine_version}/linux-x64/linux-x64-embedder.zip"

class FlutterArtifacts(ZipSolution):
    path = "depot/flutter-artifacts"
    #url = "https://storage.googleapis.com/flutter_infra_release/flutter/54a7145303f0dd9d0f93424a2e124eb4abef5091/linux-x64/artifacts.zip"
    engine_version = get_engine_version()
    url = f"https://storage.googleapis.com/flutter_infra_release/flutter/{engine_version}/linux-x64/artifacts.zip"


solutions = [GoogleTest, Spdlog, SDL, Pybind11, GLM, FlutterEngine, FlutterArtifacts]
