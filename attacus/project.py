from pathlib import Path

from .pyproject import PyProject

class Project:
    def __new__(cls, *args, **kwargs):
        global _project
        if not _project:
            _project = super(Project, cls).__new__(cls)
        return _project

    def __init__(self, path: Path) -> None:
        self.path: Path = path
        self.pyproject:PyProject = PyProject.load(path)
        self.name: str = self.pyproject.name

    @property
    def flutter_assets(self):
        if hasattr(self.pyproject.tool.attacus, 'flutter_assets'):
            return Path(self.pyproject.tool.attacus.flutter_assets)
        return Path.cwd() / 'flutter_assets'

_project: Project = None

def get_project() -> Project:
    global _project
    if not _project:
        _project = Project(Path.cwd())
    return _project