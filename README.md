# Native OpenCV

A wrapped around the OpenCV library to achieve [Lucas-Kanade Optical Flow](https://docs.opencv.org/3.4/d4/dee/tutorial_optical_flow.html#:~:text=Lucas%2DKanade%20Optical%20Flow%20in%20OpenCV).

Implemented using [the youtube tutorial](https://www.youtube.com/watch?v=wGH_KWpaAVM&ab_channel=TheCodingNotebook) and [code](https://github.com/ValYouW/flutter-opencv-stream-processing/blob/master/tutorial.md) from **The Coding Notebook**.

When cloning this repo, make sure to also download the Android SDK from [Source forge](https://sourceforge.net/projects/opencvlibrary/files/4.5.5/opencv-4.5.5-android-sdk.zip/download).

With this installed, note the path that the unzipped sdk is located at. Paste this path into the [`CMakeLists.txt`](android/CMakeLists.txt) file in the `android` folder, on line `6`. Ensure the path uses the `/` path separator.
