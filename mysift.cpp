#include "mysift.h"


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>

MySIFT::MySIFT()
{

}

void MySIFT::getSIFTKps(Mat frame, vector<vector<KeyPoint> > &kPointsVect, vector<Mat> &descriptors, vector<vector<Point> > regions){
    if( !frame.empty()){
        SiftFeatureDetector sift;

        for(unsigned i = 0; i < regions.size(); i++){
            if(!regions[i].empty()){
                Mat mask(frame.size(), CV_8UC1, Scalar::all(0));
                mask(boundingRect(regions[i])).setTo(Scalar::all(255));
                vector<KeyPoint> extraKeyPoint;
                Mat descriptor;
                sift.detect(frame, extraKeyPoint, mask);
                sift.compute(frame,extraKeyPoint, descriptor);
                kPointsVect.push_back(extraKeyPoint);
                descriptors.push_back(descriptor);
            }
        }
    }
}

void MySIFT::drawSIFTKps(Mat frame, vector<vector<KeyPoint> > kPointsVect, Mat &processedFrame){
    for(unsigned i = 0; i < kPointsVect.size(); i++){
        drawKeypoints(frame, kPointsVect[i], processedFrame, Scalar::all(-1),
                      DrawMatchesFlags().DRAW_RICH_KEYPOINTS );
    }
}
