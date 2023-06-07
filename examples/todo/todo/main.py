import os
import importlib.resources as pkg_resources
from pathlib import Path
from loguru import logger

# This needs to be set before the first import of attacus
os.environ['FLUTTER_ASSETS'] = str(Path(pkg_resources.files('todo')) / 'flutter_assets')

from attacus import App, FlutterView, StandardMethodChannel, StandardMethod

from .protos.todo.todo_pb2 import TodoItem, TodoItems

_items: TodoItems = TodoItems(items=[TodoItem(id='0', title='Buy Milk')])

class MyFlutter(FlutterView):
    def __init__(self, parent):
        super().__init__(parent)
        self.channel = None

    def startup(self):
        logger.debug("Starting up Flutter ...")
        messenger = self.messenger
        logger.debug(messenger)

        channel = self.channel = StandardMethodChannel(messenger, "todo")

        StandardMethod(
            channel,
            "getTodoItems",
            lambda method_call, result: self.get_todo_items(method_call, result)
        )
        StandardMethod(
            channel,
            "addTodoItem",
            lambda method_call, result: self.add_todo_item(method_call, result)
        )

        logger.debug(self.channel)


    def shutdown(self):
        logger.debug("Shutting down Flutter ...")

    def get_todo_items(self, method_call, result):
        result.success(_items.SerializeToString())


    def add_todo_item(self, method_call, result):
        args = method_call.arguments()
        logger.debug(args)
        pyarg = args.decode()
        logger.debug(pyarg)
        item: TodoItem = TodoItem.FromString(pyarg)
        logger.debug(item)
        result.success(item.SerializeToString())

class MyApp(App):
    def __init__(self):
        super().__init__()

    def startup(self):
        logger.debug("Starting up App ...")

    def shutdown(self):
        logger.debug("Shutting down App ...")

def main():
    app = MyApp()

    flutter = MyFlutter(app)

    app.run()
