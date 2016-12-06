#include "mymser.h"

MyMSER::MyMSER(int _delta, int _min_area, int _max_area,
               double _max_variation, float mThreshold){
    this->_delta = _delta;
    this->_min_area = _min_area;
    this->_max_area = _max_area;
    this->_max_variation = _max_variation;
    this->mThreshold = mThreshold;
}

void MyMSER::getMSERs(Mat &frame, vector< vector<Point> > &regions){

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
