#include <opencv2/core.hpp>
#include "OpticalFlow.h"

using namespace std;
using namespace cv;

void rotateMat(Mat &matImage, int rotation) {

}

extern "C" {
    // Attributes to prevent unused functions from being removed
    __attribute__((visibility("default"))) __attribute__((used))
    const char *version() {
        return CV_VERSION;
    }
}