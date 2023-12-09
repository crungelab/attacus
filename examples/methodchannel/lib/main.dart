import 'dart:developer';

import 'package:flutter/services.dart';
import 'package:flutter/material.dart';
import 'control.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  const MyApp({Key? key}) : super(key: key);
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData.dark(useMaterial3: true),
      home: const MyHomePage(title: 'Attacus Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  ToggleControl toggleControl = ToggleControl('example');
  int _counter = 0;
  static const platform = MethodChannel('example');

  /*void _incrementCounter() {
    setState(() {
      _counter++;
    });
  }*/

  Future<void> _incrementCounter() async {
    int response = 0;
    try {
      final int result = await platform.invokeMethod('incrementCounter');
      response = result;
    } on PlatformException catch (e) {
      log("Failed to Invoke: '${e.message}'.");
    }
    setState(() {
      _counter = response;
    });
  }

  void toggle() async {}

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            const TextField(
              decoration: InputDecoration(
                border: OutlineInputBorder(),
                hintText: 'Enter a search term',
              ),
            ),
            toggleControl.build((context, snapshot) {
              return SwitchListTile(
                title: const Text('Bypass'),
                value: snapshot.data ?? false,
                onChanged: (bool value) {
                  toggleControl.setValue(value);
                },
                secondary: const Icon(Icons.lightbulb_outline),
              );
            }),
            //Toggle(),
            const Text(
              'You have pushed the button this many times:',
            ),
            Text(
              '$_counter',
              style: Theme.of(context).textTheme.headlineMedium,
            ),
          ],
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: _incrementCounter,
        tooltip: 'Increment',
        child: const Icon(Icons.add),
      ), // This trailing comma makes auto-formatting nicer for build methods.
    );
  }
}
