cmake_minimum_required(VERSION 3.18.1)

# open_cv
set(OpenCV_STATIC ON)
# OpenCV path is second arg below
set(OpenCV_DIR D:/OpenCV-android-sdk/sdk/native/jni)
find_package (OpenCV REQUIRED)

add_library( #Sets the name of the library
    native_opencv
    SHARED
    # provides a relative path to source files
    ../ios/Classes/OpticalFlow/OpticalFlow.cpp
    ../ios/Classes/native_opencv.cpp
)

target_include_directories(
    native_opencv PRIVATE
    ../ios/Classes/OpticalFlow
)

find_library(log-lib log)

target_link_libraries( #specifies the target library
    native_opencv
    ${OpenCV_LIBS}
    ${log-lib}
)