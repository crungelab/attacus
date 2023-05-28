import shutil
from pathlib import Path

from rich import print

from .artifacts import get_icu_data_path

def post_build():
    # Copy icudtl.dat to the build folder so c++ tests and examples can find it
    from_path = get_icu_data_path()
    to_path = Path.cwd() / 'build'
    shutil.copy2(from_path, to_path)

if __name__ == "__main__":
    post_build()
