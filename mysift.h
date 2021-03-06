#ifndef MYSIFT_H
#define MYSIFT_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>


using namespace cv;
using namespace std;

class MySIFT
{
public:
    MySIFT();
    void getDescriptors(vector< KeyPoint> keyPoints, vector<Mat> keyPointFrames, Mat &descriptors);
    void drawKeyPoints(Mat frame, vector<KeyPoint> kPointsVect, Mat &processedFrame);
};

#endif // MYSIFT_H
