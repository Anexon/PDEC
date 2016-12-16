#include "mysift.h"


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>

MySIFT::MySIFT()
{

}

void MySIFT::getSIFTKps(Mat frame, vector<KeyPoint> &kPointsVect, Mat &descriptors, vector<vector<Point> > regions){
    if( !frame.empty()){
        SiftFeatureDetector sift;

        for(unsigned i = 0; i < regions.size(); i++){
            if(!regions[i].empty()){
                RotatedRect rct = fitEllipse(regions[i]);
                //Mat mask(frame.size(), CV_8UC1, Scalar::all(0));
                //mask(boundingRect(rct).setTo(Scalar::all(255));

                // Initialize container to apply transformations
                //Mat container;
                //frame.copyTo(container, mask);

                // Get rotation matrix without scale
                Mat tMatrix = getRotationMatrix2D(rct.center, rct.angle, 1);

                // Perform affine transformation
                Mat containerTransformed;
                warpAffine(frame, containerTransformed, tMatrix, frame.size());
                // Get final rect
                Mat finalContainer;
                if(rct.size.width > 1 && rct.size.height != 0){
                    getRectSubPix(containerTransformed, rct.size, rct.center, finalContainer);

                    // Normalize Ellipse
                    Mat keyPointRegion;
                    Size circleSize(rct.size.width/2, rct.size.width/2);
                    resize(finalContainer, keyPointRegion, circleSize);

                    // Define KeyPoint
                    vector<KeyPoint> auxKeyPointVector;
                    // TODO: Crash sometimes
                    // /build/opencv-cGZqi3/opencv-2.4.9.1+dfsg/modules/imgproc/src/imgwarp.cpp:1835: error: (-215) dsize.area() || (inv_scale_x > 0 && inv_scale_y > 0) in function resize
                    KeyPoint keyPoint(keyPointRegion.cols/2, keyPointRegion.rows/2, circleSize.width);
                    auxKeyPointVector.push_back(keyPoint);
                    // Get descriptors
                    Mat descriptor;
                    //sift.detect(frame, extraKeyPoint, mask);
                    sift.compute(keyPointRegion,auxKeyPointVector, descriptor);
                    kPointsVect.push_back(keyPoint);
                    descriptors.push_back(descriptor.row(0));

                    // Clear content for next loop
                    tMatrix.release();
                    containerTransformed.release();
                    finalContainer.release();
                    keyPointRegion.release();
                    auxKeyPointVector.clear();
                    descriptor.release();
                }
            }
        }
    }
}

void MySIFT::drawSIFTKps(Mat frame, vector<KeyPoint> kPointsVect, Mat &processedFrame){
    drawKeypoints(frame, kPointsVect, processedFrame, Scalar::all(-1),
                  DrawMatchesFlags().DRAW_RICH_KEYPOINTS );
}

/*
// Initialize container to apply transformations
Mat container;
frame.copyTo(container, mask);

// Get rotation matrix without scale
Mat tMatrix = getRotationMatrix2D(rct.center, rct.angle, 1);

// Perform affine transformation
Mat containerTransformed;
warpAffine(container, containerTransformed, tMatrix);

// Get final rect
Mat finalContainer;
getRectSubPix(containerTransformed, rct.size, rct.center, finalContainer);

// Normalize Ellipse
Size circleSize(rct.size.width/2, rct.size.width/2);
resize(finalContainer, keyPointRegion, circleSize);
// Get descriptors
*/
