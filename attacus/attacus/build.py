import os, sys
from pathlib import Path
import subprocess

from loguru import logger

from .post_build import post_build
from .install import install

FLUTTER = 'flutter.bat' if sys.platform == 'win32' else 'flutter'

def build():
    args = [FLUTTER, 'build', 'bundle']
    flutter_engine = os.environ.get('FLUTTER_ENGINE')
    if flutter_engine:
        local_engine = os.environ.get('LOCAL_ENGINE')
        args.append(f'--local-engine={local_engine}')
    subprocess.run(
        args,
        cwd = Path.cwd(), check = True)
    
    post_build()
    install()
    
if __name__ == "__main__":
    build()
