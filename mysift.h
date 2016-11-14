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
    void getSIFTKps(Mat frame, vector<vector<KeyPoint> > &kPointsVect, vector<vector<Point> > regions);
    void drawSIFTKps(Mat frame, vector<vector<KeyPoint> > kPointsVect, Mat &processedFrame);
};

#endif // MYSIFT_H
