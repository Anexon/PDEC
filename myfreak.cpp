#include "myfreak.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>

MyFREAK::MyFREAK()
{

}

void MyFREAK::getDescriptors(vector< KeyPoint> keyPoints, vector<Mat> keyPointFrames, Mat &descriptors){
    FREAK freakDescriptor; // Default Settings

    for(unsigned i = 0; i < keyPoints.size(); i++){

        // Get descriptors
        Mat descriptor;
        vector<KeyPoint> auxKeyPoints;
        auxKeyPoints.push_back(keyPoints[i]);

        // Generates descriptor of given region
        cout << "\nFrame Size " << keyPointFrames[i].rows << "x" << keyPointFrames[i].cols;
        freakDescriptor.compute(keyPointFrames[i], auxKeyPoints, descriptor);

        // Stores descriptor
        cout << "\n1";
        descriptors.push_back(descriptor.row(0));
        cout << "\n1";

        descriptor.release();
        auxKeyPoints.clear();
    }
}

void MyFREAK::drawKeyPoints(Mat frame, vector<KeyPoint> kPointsVect, Mat &processedFrame){
    drawKeypoints(frame, kPointsVect, processedFrame, Scalar::all(-1),
                  DrawMatchesFlags().DRAW_RICH_KEYPOINTS );
}

