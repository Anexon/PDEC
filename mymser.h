#ifndef MYMSER_H
#define MYMSER_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>

using namespace cv;
using namespace std;

class MyMSER
{
    public:
        MyMSER(int _delta=5, int _min_area=60, int _max_area=14400,
               double _max_variation=0.25);
        void getMSERs(Mat &frame, float mThreshold, vector< vector<Point> > &regions);
        void plotMSER(Mat frame, vector< vector<Point> > regions, Mat &processedFrame);
    private:
        int _delta;
        int _min_area;
        int _max_area;
        double _max_variation;
};

#endif // MYMSER_H
