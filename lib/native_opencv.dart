import 'dart:ffi';
import 'dart:io';

import 'package:ffi/ffi.dart';
import 'package:flutter/services.dart';

import 'native_opencv_platform_interface.dart';

// Load the C library
final DynamicLibrary nativeOpenCVLib = Platform.isAndroid
    ? DynamicLibrary.open("libnative_opencv.so")
    : DynamicLibrary.process();

// Define the C function
typedef _c_version = Pointer<Utf8> Function();

// Dart function signatures
typedef _dart_version = Pointer<Utf8> Function();

// Create dart functions that invoke the C functions
final _version =
    nativeOpenCVLib.lookupFunction<_c_version, _dart_version>("version");

class NativeOpencv {
  Future<String?> getPlatformVersion() {
    return NativeOpencvPlatform.instance.getPlatformVersion();
  }

  static const MethodChannel _channel = MethodChannel('native_opencv');

  static Future<String> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version!;
  }

  String cvVersion() {
    return _version().toDartString();
  }
}
