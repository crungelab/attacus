import shutil
from pathlib import Path
import json

from rich import print

from .artifacts import get_icu_data_path, get_engine_version

def post_build():
    engine_data =  {
        "version": get_engine_version()
    }
    engine_data_path = Path.cwd() / 'build' / 'flutter_assets' / 'Engine.json'
    with open(engine_data_path, 'w') as f:
        json.dump(engine_data, f)

    # Copy icudtl.dat to the build folder so c++ tests and examples can find it
    from_path = get_icu_data_path()
    to_path = Path.cwd() / 'build'
    shutil.copy2(from_path, to_path)

if __name__ == "__main__":
    post_build()
