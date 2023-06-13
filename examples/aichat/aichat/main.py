import os
import inspect
import asyncio
from pathlib import Path
import importlib.resources as pkg_resources

from dotenv import load_dotenv

load_dotenv()  # take environment variables from .env.

from loguru import logger

# This needs to be set before the first import of attacus
flutter_assets_path = Path(pkg_resources.files('aichat')) / 'flutter_assets'
logger.debug(flutter_assets_path)
os.environ['FLUTTER_ASSETS'] = str(flutter_assets_path)

from attacus import App, FlutterView, StandardMethodChannel, StandardMethod

from langchain.chat_models import ChatOpenAI, ChatGooglePalm
from langchain.chains import ConversationChain
from langchain.memory import ConversationBufferMemory

llm = ChatGooglePalm()
#llm = ChatOpenAI()

memory = ConversationBufferMemory()
chain = ConversationChain(llm=llm, memory=memory, verbose=True)

class MyChannel(StandardMethodChannel):
    routes = {}

    def __init__(self, messenger, name, receiver):
        super().__init__(messenger, name)
        self.receiver = receiver
        for key, value in self.routes.items():
            def execute_method(method_call, result, value=value):
                self.execute(method_call, value, result)
            StandardMethod(
                self,
                key,
                lambda method_call, result, execute_method=execute_method: execute_method(method_call, result)
            )

    @classmethod
    def route(cls, method_name):
        def decorator(f):
            cls.routes[method_name] = f
            return f
        return decorator

    def execute(self, method_call, method, method_result):
        logger.debug(method_call)
        logger.debug(method)
        logger.debug(method_call.method_name())
        args = method_call.arguments()
        logger.debug(args)
        logger.debug(method_result)

        pyarg = args.decode()
        logger.debug(pyarg)

        if inspect.iscoroutinefunction(method):
            loop = asyncio.get_event_loop()
            def callback(future):
                result = future.result()
                logger.debug(f"Callback invoked with result: {result}")
                method_result.success(result)
            task = loop.create_task(method(self.receiver, pyarg))
            # Add the callback
            task.add_done_callback(callback)
        else:
            result = method(self.receiver, pyarg)
            method_result.success(result)

class MyFlutter(FlutterView):
    def __init__(self, parent):
        super().__init__(parent)
        self.channel = None

    def startup(self):
        super().startup()
        logger.debug("Starting up Flutter ...")
        messenger = self.messenger
        logger.debug(messenger)

        self.channel = MyChannel(messenger, "aichat", self)
        logger.debug(self.channel)


    def shutdown(self):
        super().shutdown()
        logger.debug("Shutting down Flutter ...")

    @MyChannel.route('send')
    async def send(self, text: str):
        logger.debug(text)
        try:
            response = await chain.arun(text)
        except Exception as e:
            msg = f"An error occurred: {e}"
            logger.error(msg)
            response = msg

        self.channel.invoke_method('on_message', response)

class MyApp(App):
    def __init__(self):
        super().__init__()
        self.flutter = MyFlutter(self)

    def startup(self):
        super().startup()
        logger.debug("Starting up App ...")

    """
    # Synchronous event loop
    def loop(self):
        logger.debug("Entering App Loop ...")
        while self.process_events():
            pass
        logger.debug("Exiting App Loop ...")
    """

    # Asynchronous event loop
    def loop(self):
        logger.debug("Entering App Loop ...")
        def exception_handler(loop, context):
            print(f"Caught exception: {context['exception']}")
        async def _loop(interval=1/60):
            loop = asyncio.get_event_loop()
            loop.set_exception_handler(exception_handler)
            while self.process_events():
                await asyncio.sleep(interval)

        asyncio.run(_loop())
        logger.debug("Exiting App Loop ...")

    def shutdown(self):
        super().shutdown()
        logger.debug("Shutting down App ...")

def main():
    app = MyApp()
    app.run()
