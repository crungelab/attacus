import os
import requests
import tempfile
import zipfile
import json
import __main__
from loguru import logger

from pathlib import Path
from platformdirs import user_cache_dir

appname = "Attacus"
appauthor = "CrungeLab"


def ensure() -> None:
    logger.debug('ensure')
    version = get_engine_version()
    if not version:
        raise RuntimeError('Flutter Engine Version not found')
    ensure_engine(version)
    ensure_artifacts(version)

def ensure_engine(version: str) -> None:
    path = Path(user_cache_dir(appname, appauthor)) / 'flutter' / version / 'engine'
    #logger.debug(path)
    if not path.exists():
        url = f'https://storage.googleapis.com/flutter_infra_release/flutter/{version}/windows-x64/windows-x64-embedder.zip'
        zip_file = download_file(url)
        extract_zip(zip_file, path)

    #os.environ["PATH"] = f"{path}{os.pathsep}{os.environ['PATH']}"
    os.add_dll_directory(path)

def ensure_artifacts(version: str) -> None:
    path = Path(user_cache_dir(appname, appauthor)) / 'flutter' / version / 'artifacts'
    #logger.debug(path)
    if not path.exists():
        url = f'https://storage.googleapis.com/flutter_infra_release/flutter/{version}/windows-x64/artifacts.zip'
        zip_file = download_file(url)
        extract_zip(zip_file, path)

_engine_version: str = None

def get_engine_version() -> str:
    global _engine_version
    if _engine_version:
        return _engine_version

    version = None
    flutter_assets = os.environ.get('FLUTTER_ASSETS')
    if flutter_assets:
        version = get_app_engine_version()
    else:
        version = get_sdk_engine_version()
    logger.debug(version)
    _engine_version = version
    return version

def get_app_engine_version() -> str:
    flutter_assets = os.environ.get('FLUTTER_ASSETS')
    if not flutter_assets:
        return None
    logger.debug(f'loading engine version from {flutter_assets}')
    engine_data_path = Path(flutter_assets) / 'Engine.json'
    with open(engine_data_path, 'r') as f:
        engine_data = json.load(f)
        version = engine_data['version']
    return version

def get_sdk_engine_version() -> str:
    flutter_root = os.environ.get('FLUTTER_ROOT')
    if not flutter_root:
        return None
    logger.debug(f'loading engine version from sdk')
    version_path = flutter_root / Path('bin/internal/engine.version')
    with open(version_path, 'r') as file:
        version = file.readline().rstrip()
    return version

def download_file(url: str):
    response = requests.get(url, stream=True)
    temp_file = tempfile.TemporaryFile()
    for chunk in response.iter_content(chunk_size=128):
        temp_file.write(chunk)
    # reset file pointer to beginning
    temp_file.seek(0)
    return temp_file

def extract_zip(file_path, extract_path):
    with zipfile.ZipFile(file_path, 'r') as zip_ref:
        zip_ref.extractall(extract_path)

def get_icu_data_path():
    version = get_engine_version()
    path = Path(user_cache_dir(appname, appauthor)) / 'flutter' / version / 'artifacts' / 'icudtl.dat'
    return path