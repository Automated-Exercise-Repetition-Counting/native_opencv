#include "OpticalFlow.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

using namespace std;
using namespace cv;

#define MOVEMENT_THRESHOLD 0.2
#define FRAMES_BEFORE_RECALC_GOOD_POINTS 10
#define ERROR_VALUE std::numeric_limits<float>::infinity()

void OpticalFlowCalculator::init(cv::Mat frame)
{
    goodFeaturesToTrack(frame, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
    old_gray = frame;
}

cv::Point2f OpticalFlowCalculator::process(cv::Mat frame)
{
    recalcPointsCounter++;
    if (recalcPointsCounter > FRAMES_BEFORE_RECALC_GOOD_POINTS || p0.size() <= 0)
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
        // sets x and y to infinity initially
        float xSummation = 0;
        float ySummation = 0;
        int movingPointCount = 0;

        for (uint i = 0; i < p0.size(); i++)
        {
            // Select good points
            if (status[i] == 1)
            {
                good_new.push_back(p1[i]);

                float newX = p1[i].x - p0[i].x;
                float newY = p1[i].y - p0[i].y;

                bool newXAboveThrshold = abs(newX) > MOVEMENT_THRESHOLD;
                bool newYAboveThrshold = abs(newY) > MOVEMENT_THRESHOLD;

                if (newXAboveThrshold)
                {
                    xSummation += newX;
                }

                if (newYAboveThrshold)
                {
                    ySummation += newY;
                }

                if (newXAboveThrshold || newYAboveThrshold)
                {
                    movingPointCount++;
                }
            }
        }

        // avoids division by zero error
        movingPointCount = movingPointCount == 0 ? 1 : movingPointCount;
        result.x = xSummation / movingPointCount;
        result.y = ySummation / movingPointCount;

        // update old frame
        p0 = good_new;
        old_gray = frame.clone();
    }
    catch (Exception e)
    {
        // set result to error value
        result.x = ERROR_VALUE;
        result.y = ERROR_VALUE;
    }
    return result;
};