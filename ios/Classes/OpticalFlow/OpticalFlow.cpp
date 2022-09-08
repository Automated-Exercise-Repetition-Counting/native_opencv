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

cv::Point2f OpticalFlowCalculator::process(cv::Mat frame)
{
    if (recalcPointsCounter > 10 || p0.size() <= 0)
    {
        init(old_gray);
        recalcPointsCounter = 0;
    }

    // calculate optical flow
    std::vector<uchar> status;
    std::vector<float> err;
    std::vector<cv::Point2f> p1;
    cv::Point2f result;

    try
    {
        TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
        calcOpticalFlowPyrLK(old_gray, frame, p0, p1, status, err, Size(15, 15), 2, criteria);
        std::vector<cv::Point2f> good_new;
        // Calculate average of all points in p1 and p0
        float x = 0;
        float y = 0;

        for (uint i = 0; i < p0.size(); i++)
        {
            // Select good points
            if (status[i] == 1)
            {
                good_new.push_back(p1[i]);
                x += p1[i].x - p0[i].x;
                y += p1[i].y - p0[i].y;
            }
        }

        x /= good_new.size();
        y /= good_new.size();

        result.x = x;
        result.y = y;

        // update old frame
        p0 = good_new;
        old_gray = frame.clone();
    }
    catch (Exception e)
    {
        ;
    }
    return result;
};