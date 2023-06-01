import os
from pathlib import Path

def get_assets_path():
    return os.environ.get('FLUTTER_ASSETS') or Path.cwd() / 'flutter_assets'