from pathlib import Path
from importlib import resources

from loguru import logger

#from .attacus import *
from . import attacus as core
from .flutter_config import FlutterConfig

class FlutterView(core.FlutterView):
    def __init__(self, app :core.App, config: FlutterConfig = FlutterConfig()) -> None:
        super().__init__(app, config)