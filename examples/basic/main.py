import os
from pathlib import Path

# This needs to be set before the first import of attacus
os.environ['FLUTTER_ASSETS'] = str(Path(__file__).parent / 'flutter_assets')

from attacus import App, FlutterView

app = App()

flutter = FlutterView(app)

app.run()