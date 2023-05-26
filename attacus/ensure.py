import os
import requests
import tempfile
import zipfile

from loguru import logger

from pathlib import Path
from platformdirs import user_cache_dir

appname = "Attacus"
appauthor = "CrungeLab"


def ensure() -> None:
    id = get_engine_id()
    if not id:
        raise RuntimeError('Flutter Engine ID not found')
    path = Path(user_cache_dir(appname, appauthor)) / 'engines' / id
    logger.debug(path)
    if not path.exists():
        url = f'https://storage.googleapis.com/flutter_infra_release/flutter/{id}/windows-x64/windows-x64-embedder.zip'
        zip_file = download_file(url)
        extract_zip(zip_file, path)

    #os.environ["PATH"] = f"{path}{os.pathsep}{os.environ['PATH']}"
    os.add_dll_directory(path)

def get_engine_id() -> str:
    #id = '90fa3ae28fe6ddaee1af2c120f01e50201c1401b'
    id = os.environ.get('FLUTTER_ENGINE_ID')
    if not id:
        flutter_root = os.environ.get('FLUTTER_ROOT')
        if not flutter_root:
            return None
        version_path = flutter_root / Path('bin/internal/engine.version')
        with open(version_path, 'r') as file:
            id = file.readline().rstrip()
    logger.debug(id)
    return id

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
