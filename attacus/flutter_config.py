from pathlib import Path
from importlib import resources

from loguru import logger

from . import attacus as core
from .artifacts import get_icu_data_path

class FlutterConfig(core.FlutterConfig):
    def __init__(self) -> None:
        super().__init__()
        self.icu_data_path = str(get_icu_data_path())
        logger.debug(self.icu_data_path)
