import shutil
from pathlib import Path

from rich import print

def install():
    from_path = Path.cwd() / 'build' / 'flutter_assets'
    to_path = Path.cwd() / 'flutter_assets'
    #print('[green]Installing :thumbs_up:') # This will bomb with F5 in vscode
    print(f'[green]Installing flutter_assets to {to_path}[/green]')
    if to_path.exists():
        shutil.rmtree(to_path)
    shutil.copytree(from_path, to_path)

if __name__ == "__main__":
    install()
