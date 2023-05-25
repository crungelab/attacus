from procure import GitSolution

"""
[submodule "depot/sdl"]
	path = depot/sdl
	url = https://github.com/libsdl-org/SDL
[submodule "depot/glad"]
	path = depot/glad
	url = https://github.com/Dav1dde/glad
	branch = glad2
[submodule "depot/googletest"]
	path = depot/googletest
	url = https://github.com/google/googletest
[submodule "depot/glm"]
	path = depot/glm
	url = https://github.com/g-truc/glm
[submodule "depot/pybind11"]
	path = depot/pybind11
	url = https://github.com/pybind/pybind11
"""

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

solutions = [
	GoogleTest,
    SDL,
    Glad,
    Pybind11,
    GLM
]
