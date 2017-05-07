#include "mysift.h"


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>

MySIFT::MySIFT()
{

}

void MySIFT::getDescriptors(vector< KeyPoint> keyPoints, vector<Mat> keyPointFrames, Mat &descriptors){
    SiftFeatureDetector sift;

    for(unsigned i = 0; i < keyPoints.size(); i++){

        // Get descriptors
        Mat descriptor;
        vector<KeyPoint> auxKeyPoints;
        auxKeyPoints.push_back(keyPoints[i]);

        // Generates descriptor of given region
        // cout << "\nFrame Size " << keyPointFrames[i].rows << "x" << keyPointFrames[i].cols;
        sift.compute(keyPointFrames[i], auxKeyPoints, descriptor);
        // Stores descriptor
        descriptors.push_back(descriptor.row(0));

        descriptor.release();
        auxKeyPoints.clear();
    }
}

void MySIFT::drawKeyPoints(Mat frame, vector<KeyPoint> kPointsVect, Mat &processedFrame){
    drawKeypoints(frame, kPointsVect, processedFrame, Scalar::all(-1),
                  DrawMatchesFlags().DRAW_RICH_KEYPOINTS );
}
