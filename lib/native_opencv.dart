import 'dart:ffi';
import 'dart:io';
import 'dart:typed_data';

import 'package:ffi/ffi.dart';
import 'package:flutter/services.dart';

import 'native_opencv_platform_interface.dart';

// Load the C library
final DynamicLibrary nativeOpenCVLib = Platform.isAndroid
    ? DynamicLibrary.open("libnative_opencv.so")
    : DynamicLibrary.process();

// Define the C function
typedef _c_version = Pointer<Utf8> Function();
typedef _c_initCalculator = Void Function(Int32 width, Int32 height,
    Int32 rotation, Pointer<Uint8> bytes, Bool isYuV);
typedef _c_opticalFlowIteration = Pointer<Float> Function(
    Int32 width,
    Int32 height,
    Int32 rotation,
    Pointer<Uint8> bytes,
    Bool isYuV,
    Pointer<Int32> outCount);

// Dart function signatures
typedef _dart_version = Pointer<Utf8> Function();
typedef _dart_initCalculator = void Function(
    int width, int height, int rotation, Pointer<Uint8> bytes, bool isYUV);
typedef _dart_opticalFlowIteration = Pointer<Float> Function(
    int width,
    int height,
    int rotation,
    Pointer<Uint8> bytes,
    bool isYUV,
    Pointer<Int32> outCount);

// Create dart functions that invoke the C functions
final _version =
    nativeOpenCVLib.lookupFunction<_c_version, _dart_version>("version");
final _initOpticalFloatCalculator = nativeOpenCVLib
    .lookupFunction<_c_initCalculator, _dart_initCalculator>("initCalculator");
final _opticalFlowIteration = nativeOpenCVLib.lookupFunction<
    _c_opticalFlowIteration,
    _dart_opticalFlowIteration>("opticalFlowIteration");

class NativeOpencv {
  Future<String?> getPlatformVersion() {
    return NativeOpencvPlatform.instance.getPlatformVersion();
  }

  Pointer<Uint8>? _imageBuffer;

  static const MethodChannel _channel = MethodChannel('native_opencv');

  static Future<String> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version!;
  }

  String cvVersion() {
    return _version().toDartString();
  }

  void initCalculator(int width, int height, int rotation, Uint8List yBuffer,
      Uint8List? uBuffer, Uint8List? vBuffer) {
    var ySize = yBuffer.lengthInBytes;
    var uSize = uBuffer?.lengthInBytes ?? 0;
    var vSize = vBuffer?.lengthInBytes ?? 0;
    var totalSize = ySize + uSize + vSize;

    _imageBuffer ??= malloc.allocate<Uint8>(totalSize);

    // We always have at least 1 plane, on Android it si the yPlane on iOS its the rgba plane
    Uint8List bytes = _imageBuffer!.asTypedList(totalSize);
    bytes.setAll(0, yBuffer);

    if (Platform.isAndroid) {
      // Swap u&v buffer for opencv
      bytes.setAll(ySize, vBuffer!);
      bytes.setAll(ySize + vSize, uBuffer!);
    }

    _initOpticalFloatCalculator(width, height, rotation, _imageBuffer!,
        Platform.isAndroid ? true : false);
  }

  Float32List opticalFlowIteration(int width, int height, int rotation,
      Uint8List yBuffer, Uint8List? uBuffer, Uint8List? vBuffer) {
    var ySize = yBuffer.lengthInBytes;
    var uSize = uBuffer?.lengthInBytes ?? 0;
    var vSize = vBuffer?.lengthInBytes ?? 0;
    var totalSize = ySize + uSize + vSize;

    _imageBuffer ??= malloc.allocate<Uint8>(totalSize);

    // We always have at least 1 plane, on Android it si the yPlane on iOS its the rgba plane
    Uint8List bytes = _imageBuffer!.asTypedList(totalSize);
    bytes.setAll(0, yBuffer);

    if (Platform.isAndroid) {
      // Swap u&v buffer for opencv
      bytes.setAll(ySize, vBuffer!);
      bytes.setAll(ySize + vSize, uBuffer!);
    }

    Pointer<Int32> outCount = malloc.allocate<Int32>(1);
    var res = _opticalFlowIteration(width, height, rotation, _imageBuffer!,
        Platform.isAndroid ? true : false, outCount);
    final count = outCount.value;
    malloc.free(outCount);

    return res.asTypedList(count);
  }
}
