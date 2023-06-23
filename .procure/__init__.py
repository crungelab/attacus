from procure import GitSolution, ZipSolution


class GoogleTest(GitSolution):
	path = 'depot/googletest'
	url = 'https://github.com/google/googletest'

class Spdlog(GitSolution):
	path = 'depot/spdlog'
	url = 'https://github.com/gabime/spdlog'

class SDL(GitSolution):
	path = 'depot/sdl'
	url = 'https://github.com/libsdl-org/SDL'

class Glad(GitSolution):
	path = 'depot/glad'
	url = 'https://github.com/Dav1dde/glad'

class Pybind11(GitSolution):
	path = 'depot/pybind11'
	url = 'https://github.com/pybind/pybind11'

class GLM(GitSolution):
	path = 'depot/glm'
	url = 'https://github.com/g-truc/glm'

#TODO: Need to make this multi-platform
class FlutterEngine(ZipSolution):
	path = 'depot/flutter-engine'
	url = 'https://storage.googleapis.com/flutter_infra_release/flutter/45f6e009110df4f34ec2cf99f63cf73b71b7a420/linux-x64/linux-x64-embedder.zip'

solutions = [
	GoogleTest,
	Spdlog,
    SDL,
    Glad,
    Pybind11,
    GLM,
    FlutterEngine
]
