import sys
import shutil
from pathlib import Path
import subprocess

from .post_build import post_build
from .install import install

FLUTTER = 'flutter.bat' if sys.platform == 'win32' else 'flutter'

def build():
    subprocess.run(
        [FLUTTER, 'build', 'bundle'],
        cwd = Path.cwd(), check = True)
    
    post_build()
    install()
    
if __name__ == "__main__":
    build()
