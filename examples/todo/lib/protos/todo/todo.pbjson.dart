///
//  Generated code. Do not modify.
//  source: todo/todo.proto
//
// @dart = 2.12
// ignore_for_file: annotate_overrides,camel_case_types,constant_identifier_names,deprecated_member_use_from_same_package,directives_ordering,library_prefixes,non_constant_identifier_names,prefer_final_fields,return_of_invalid_type,unnecessary_const,unnecessary_import,unnecessary_this,unused_import,unused_shown_name

import 'dart:core' as $core;
import 'dart:convert' as $convert;
import 'dart:typed_data' as $typed_data;
@$core.Deprecated('Use todoItemDescriptor instead')
const TodoItem$json = const {
  '1': 'TodoItem',
  '2': const [
    const {'1': 'id', '3': 1, '4': 1, '5': 9, '10': 'id'},
    const {'1': 'title', '3': 2, '4': 1, '5': 9, '10': 'title'},
    const {'1': 'isDone', '3': 3, '4': 1, '5': 8, '10': 'isDone'},
  ],
};

/// Descriptor for `TodoItem`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List todoItemDescriptor = $convert.base64Decode('CghUb2RvSXRlbRIOCgJpZBgBIAEoCVICaWQSFAoFdGl0bGUYAiABKAlSBXRpdGxlEhYKBmlzRG9uZRgDIAEoCFIGaXNEb25l');
@$core.Deprecated('Use todoItemsDescriptor instead')
const TodoItems$json = const {
  '1': 'TodoItems',
  '2': const [
    const {'1': 'items', '3': 1, '4': 3, '5': 11, '6': '.todo.TodoItem', '10': 'items'},
  ],
};

/// Descriptor for `TodoItems`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List todoItemsDescriptor = $convert.base64Decode('CglUb2RvSXRlbXMSJAoFaXRlbXMYASADKAsyDi50b2RvLlRvZG9JdGVtUgVpdGVtcw==');
