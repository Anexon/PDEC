#include "mymser.h"

MyMSER::MyMSER(int _delta, int _min_area, int _max_area,
               double _max_variation, float mThreshold){
    this->_delta = _delta;
    this->_min_area = _min_area;
    this->_max_area = _max_area;
    this->_max_variation = _max_variation;
    this->mThreshold = mThreshold;
}

void MyMSER::getMSERs(Mat &frame, vector< vector<Point> > &regions, vector< KeyPoint> &keyPoints, vector<Mat> &keyPointFrames){

    MSER ms(_delta,_min_area,_max_area,_max_variation);
    //vector< vector<Point> > regions;

    if(!frame.empty()) {
        vector< vector<Point> > obtainedRegions;
        ms.operator()(frame,obtainedRegions,Mat());

        // Calculate mean for every region and if below the threshold it is added to the final regions vector

        vector<Vec3b> intensityRegion;
        Vec3b intensityPixel;
        for(unsigned j=0; j < obtainedRegions.size(); j++){
            for(unsigned i=0; i < obtainedRegions[j].size(); i++){
                intensityPixel = frame.at<Vec3b>(obtainedRegions[j][i]);
                intensityRegion.push_back( intensityPixel );
                //cout << "Pixel " << (Vec3b)intensityPixel << "\n";
                //cout << "Coordinates: " << obtainedRegions[j][i] << "\n";
                //(Scalar)frame.at<uchar>(obtainedRegions[j][i])
                //frame.at<Vec3b>(obtainedRegions[j][i]) = 255;
            }

            if( mean(intensityRegion)[0] > mThreshold ){
                regions.push_back(obtainedRegions[j]);
                KeyPoint auxKeyPoint;
                Mat auxKeyPointFrame;
                toKeyPoint(frame, obtainedRegions[j], auxKeyPoint, auxKeyPointFrame);

                // Avoid empty data
                if(auxKeyPointFrame.cols > 0 && auxKeyPointFrame.rows > 0){
                    keyPoints.push_back(auxKeyPoint);
                    keyPointFrames.push_back(auxKeyPointFrame);
                }
            }

            //regions.push_back(obtainedRegions[j]);
            intensityRegion.clear();
        }
    }
}

void MyMSER::plotMSER(Mat frame, vector< vector<Point> > regions, Mat &processedFrame){
    if(!frame.empty()){
        for(unsigned j=0; j < regions.size(); j++){
            //rectangle(frame, boxes[j], CV_RGB(0,255,0));
            ellipse(processedFrame, fitEllipse(regions[j]), Scalar(255));
            /*
            for(unsigned i=0; i< regions[j].size(); i++){
                processedFrame.at<Vec3b>(regions[j][i]) = 255;
            }
            */
        }
    }
}

void MyMSER::toKeyPoint(Mat &frame, vector<Point> region, KeyPoint &mserKeyPoint, Mat &mserKeyPointFrame){
    if(!region.empty()){
        // Get bounding ellipse of region
        RotatedRect rct = fitEllipse(region);

        // Get rotation matrix without scale
        Mat tMatrix = getRotationMatrix2D(rct.center, rct.angle, 1);

        // Perform affine transformation
        Mat containerTransformed;
        warpAffine(frame, containerTransformed, tMatrix, frame.size());

        if(rct.size.width >= 2 && rct.size.height != 0){

            // Generates bounding ellipse and then bounding rectangle of it
            RotatedRect rct = fitEllipse(region);

            // Extract Rectangle from Frame
            Mat rotatedRectangle;
            getRectSubPix(frame, rct.size, rct.center, rotatedRectangle);

            // Rotate Rectangle
            Mat verticalRectangle;
            Mat tMatrix = getRotationMatrix2D(rct.center, rct.angle, 1);
            warpAffine(rotatedRectangle, verticalRectangle, tMatrix, rotatedRectangle.size());
            // Apply vertical affine scale
            Point2f srcQuad[4];
            Point2f dstQuad[5];

            srcQuad[0] = Point2f( 0,0 );                                                    // Top Left Corner
            srcQuad[1] = Point2f( verticalRectangle.cols - 1, 0 );                          // Top Right Corner
            srcQuad[2] = Point2f( 0, verticalRectangle.rows - 1 );                          // Bottom Left Corner
            srcQuad[2] = Point2f( verticalRectangle.cols - 1, verticalRectangle.rows - 1 ); // Bottom Right Corner

            dstQuad[0] = Point2f( 0, rct.size.width/2 );                                        // Reduce Height of Top Left Corner
            dstQuad[1] = Point2f( verticalRectangle.cols - 1, rct.size.width/2 );               // Reduce Height of Top Right Corner
            dstQuad[2] = Point2f( 0, verticalRectangle.rows - 1 );                              // Keep Bottom Left Corner
            dstQuad[2] = Point2f( verticalRectangle.cols - 1, verticalRectangle.rows - 1 );     // Keep Bottom Right Corner

            // Get the Affine Transform
            Mat warp_mat;
            warp_mat = getAffineTransform( srcQuad, dstQuad );

            // Apply Transformation
            Mat normalizedEllipse;
            warpAffine( verticalRectangle, normalizedEllipse, warp_mat, verticalRectangle.size() );

            // Define KeyPoint
            Size circleSize(rct.size.width/2, rct.size.width/2);
            vector<KeyPoint> auxKeyPointVector;
            KeyPoint keyPoint(normalizedEllipse.cols/2, normalizedEllipse.rows/2, circleSize.width);

            // Asign KeyPoint
            mserKeyPoint = keyPoint;
            mserKeyPointFrame = normalizedEllipse;
        }
    }
}
