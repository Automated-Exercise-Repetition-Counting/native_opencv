#include <opencv2/core.hpp>

using namespace std;
using namespace cv;

class OpticalFlowCalculator
{
    std::vector<cv::Point2f> p0;
    cv::Mat old_gray;

public:
    OpticalFlowCalculator(){};
    void init(cv::Mat frame);
    float *process(cv::Mat frame);
};