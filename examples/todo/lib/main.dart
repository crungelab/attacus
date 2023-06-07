import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import './protos/todo/todo.pb.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: const HomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class HomePage extends StatefulWidget {
  const HomePage({super.key, required this.title});
  final String title;
  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  static const platform = MethodChannel('todo');
  final _controller = TextEditingController();
  List<TodoItem> _items = [];

  @override
  void initState() {
    super.initState();
    _loadTodoItems();
  }

  /*Future<void> _loadTodoItems() async {
    try {
      final List<dynamic> result = await platform.invokeMethod('getTodoItems');
      setState(() {
        _items =
            result.map((dynamic item) => TodoItem.fromBuffer(item)).toList();
      });
    } on PlatformException catch (e) {
      print('Failed to load todo items: $e');
    }
  }*/

  Future<void> _loadTodoItems() async {
    try {
      final Uint8List result = await platform.invokeMethod('getTodoItems');
      final TodoItems todoItems = TodoItems.fromBuffer(result);
      setState(() {
        _items = todoItems.items;
      });
    } on PlatformException catch (e) {
      print('Failed to load todo items: $e');
    }
  }

  /*Future<void> _addTodoItem(TodoItem item) async {
    try {
      final Uint8List result =
          await platform.invokeMethod('addTodoItem', item.writeToBuffer());
      setState(() {
        _items.add(TodoItem.fromBuffer(result));
      });
    } on PlatformException catch (e) {
      print('Failed to add todo item: $e');
    }
  }*/

  Future<void> _addTodoItem(String title) async {
    try {
      final TodoItem newItem = TodoItem()..title = title;
      final Uint8List result =
          await platform.invokeMethod('addTodoItem', newItem.writeToBuffer());
      setState(() {
        _items.add(TodoItem.fromBuffer(result));
      });
    } on PlatformException catch (e) {
      print('Failed to add todo item: $e');
    }
  }

  Future<void> _showAddTodoDialog(BuildContext context) async {
    return showDialog<void>(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: const Text('Add Todo'),
          content: TextField(
            controller: _controller,
            decoration: const InputDecoration(
              labelText: 'Title',
            ),
          ),
          actions: <Widget>[
            TextButton(
              child: const Text('Cancel'),
              onPressed: () {
                Navigator.of(context).pop();
              },
            ),
            TextButton(
              child: const Text('Add'),
              onPressed: () {
                _addTodoItem(_controller.text);
                _controller.clear();
                Navigator.of(context).pop();
              },
            ),
          ],
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Todo App'),
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
      ),
      body: ListView.builder(
        itemCount: _items.length,
        itemBuilder: (context, index) {
          final item = _items[index];
          return CheckboxListTile(
            title: Text(item.title),
            value: item.isDone,
            onChanged: (bool? value) {
              // Logic to update a todo item
            },
          );
        },
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: () {
          _showAddTodoDialog(context);
        },
        child: const Icon(Icons.add),
      ),
    );
  }
}
