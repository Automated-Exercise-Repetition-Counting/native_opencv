#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <OpticalFlow.h>

using namespace std;
using namespace cv;

static OpticalFlowCalculator *calculator = nullptr;
void rotateMat(Mat &matImage, int rotation)
{
    if (rotation == 90)
    {
        transpose(matImage, matImage);
        flip(matImage, matImage, 1); // transpose+flip(1)=CW
    }
    else if (rotation == 270)
    {
        transpose(matImage, matImage);
        flip(matImage, matImage, 0); // transpose+flip(0)=CCW
    }
    else if (rotation == 180)
    {
        flip(matImage, matImage, -1); // flip(-1)=180
    }
}

Mat convertToMat(int width, int height, int rotation, uint8_t *bytes, bool isYUV)
{
    Mat frame;
    if (isYUV)
    {
        rotateMat(frame, rotation);
        cvtColor(Mat(height + height / 2, width, CV_8UC1, bytes), frame, COLOR_YUV2BGR_NV21);
    }
    else
    {
        frame = Mat(height, width, CV_8UC4, bytes);
    }
    rotateMat(frame, rotation);
    cvtColor(frame, frame, COLOR_BGRA2GRAY);
    return frame;
}

extern "C"
{
    // Attributes to prevent unused functions from being removed
    __attribute__((visibility("default"))) __attribute__((used))
    const char *
    version()
    {
        return CV_VERSION;
    }

    __attribute__((visibility("default"))) __attribute__((used)) void
    initCalculator(int width, int height, int rotation, uint8_t *bytes, bool isYUV)
    {
        if (calculator != nullptr)
        {
            delete calculator;
            calculator = nullptr;
        }

        Mat frame = convertToMat(width, height, rotation, bytes, isYUV);
        calculator = new OpticalFlowCalculator();
        calculator->init(frame);
    }

    // Attributes to prevent unused functions from being removed
    __attribute__((visibility("default"))) __attribute__((used))
    const float *
    opticalFlowIteration(int width, int height, int rotation, uint8_t *bytes, bool isYUV,
                         float *outCount)
    {
        if (calculator == nullptr)
        {
            float *jres = new float[1];
            jres[0] = 0;
            return jres;
        }

        Mat new_frame = convertToMat(width, height, rotation, bytes, isYUV);
        float *result = calculator->process(new_frame);

        vector<float> output;
        // fetch two values from result
        output.push_back(result[0]);
        output.push_back(result[1]);

        unsigned int total = sizeof(float) * output.size();
        float *outputArray = (float *)malloc(total);
        memcpy(outputArray, output.data(), total);

        *outCount = output.size();
        return outputArray;
    }
}