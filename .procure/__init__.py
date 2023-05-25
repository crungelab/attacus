from procure import GitSolution, ZipSolution


class GoogleTest(GitSolution):
	path = 'depot/googletest'
	url = 'https://github.com/google/googletest'

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

class FlutterEngine(ZipSolution):
	path = 'depot/flutter-engine'
	url = 'https://storage.googleapis.com/flutter_infra_release/flutter/90fa3ae28fe6ddaee1af2c120f01e50201c1401b/windows-x64/windows-x64-embedder.zip'

solutions = [
	GoogleTest,
    SDL,
    Glad,
    Pybind11,
    GLM,
    FlutterEngine
]
