#include <opencv2/core.hpp>

using namespace std;
using namespace cv;

class OpticalFlowCalculator
{
    std::vector<cv::Point2f> p0;
    cv::Mat old_gray;
    int recalcPointsCounter = 0;

public:
    OpticalFlowCalculator(){};
    void init(cv::Mat frame);
    cv::Point2f process(cv::Mat frame);
};