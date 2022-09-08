#include "OpticalFlow.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

using namespace std;
using namespace cv;

void OpticalFlowCalculator::init(cv::Mat frame)
{
    goodFeaturesToTrack(frame, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
    old_gray = frame;
}

float *OpticalFlowCalculator::process(cv::Mat frame)
{
    // calculate optical flow
    std::vector<uchar> status;
    std::vector<float> err;
    std::vector<cv::Point2f> p1;

    TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
    calcOpticalFlowPyrLK(old_gray, frame, p0, p1, status, err, Size(15, 15), 2, criteria);
    std::vector<cv::Point2f> good_new;
    for (uint i = 0; i < p0.size(); i++)
    {
        // Select good points
        if (status[i] == 1)
        {
            good_new.push_back(p1[i]);
            // draw the tracks
            // line(mask, p1[i], p0[i], colors[i], 2);
            // circle(frame, p1[i], 5, colors[i], -1);
        }
    }

    // TODO Detect average here, then return

    // store into result
    static float result[2] = {0, 0};

    // update old frame
    p0 = good_new;
    old_gray = frame.clone();

    return result;
};