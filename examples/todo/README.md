[Download Protocol Buffers](https://protobuf.dev/downloads/)

Install the Dart protoc plugin

```bash
dart pub global activate protoc_plugin
```

Put Dart executables on your path
```
C:\Users\kurti\AppData\Local\Pub\Cache\bin
```

Run the protoc compiler
```bash
cd protos
protoc --python_out=../todo/protos --pyi_out=../todo/protos --dart_out=../lib/protos ./todo/*.proto
```
