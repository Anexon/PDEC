/*
 * OculusTI.cpp
 *
 *  Created on: 27/07/2016
 *      Author: anexon
 */

#ifndef OCULUSTI_H_
#define OCULUSTI_H_

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#include <curl/curl.h>

#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;
#include "OculusTI.h"

Mat composeImage(Mat imageOne, Mat imageTwo){
	int totalCol = imageOne.cols+imageTwo.cols;

	int totalRow = imageOne.rows >= imageTwo.rows ? imageOne.rows : imageTwo.rows;

	Mat composedImage(Size(totalCol, totalRow), imageOne.type());

	imageOne.copyTo(composedImage(Rect(0,0,imageOne.cols, totalRow)));
	imageTwo.copyTo(composedImage(Rect(imageOne.cols,0,imageTwo.cols, totalRow)));

	return composedImage;
}

int recordThermalVideo(string destPath){
	// Entrada de imagenes
	VideoCapture inputVideo2("http://root:cuadriga@192.168.0.90/mjpg/2/video.mjpg");
	if(!inputVideo2.isOpened()){
		cout << "Thermal camera cannot be opened!";
		return -1;
	}

	Size S = Size((int)inputVideo2.get(CV_CAP_PROP_FRAME_WIDTH), (int)inputVideo2.get(CV_CAP_PROP_FRAME_HEIGHT));
	VideoWriter record(destPath, CV_FOURCC('M','J','P','G'), 30, S, true);

	if( !record.isOpened()){
		cout << "VideoWriter failed to open!\n";
		return -1;
	}

	// Matriz de imagenes Thermal
	Mat thermalFrame;

	// Bucle de adquisicion de imagenes
	for(;;){
		inputVideo2 >> thermalFrame;
		record << thermalFrame;
		if(waitKey(30)>=0) break;
	}
	return 0;
}

int recordRGBVideo(string destPath){
	// Entrada de imagenes
	VideoCapture inputVideo1("http://root:cuadriga@192.168.0.90/mjpg/1/video.mjpg");
	if(!inputVideo1.isOpened()){
		cout << "Cameras cannot be opened!";
		return -1;
	}



	Size S = Size((int) inputVideo1.get(CV_CAP_PROP_FRAME_WIDTH), (int)inputVideo1.get(CV_CAP_PROP_FRAME_HEIGHT));
	VideoWriter record(destPath, CV_FOURCC('M','J','P','G'), 25, S, true);

	if( !record.isOpened()){
		cout << "VideoWriter failed to open!\n";
		return -1;
	}


	// Matriz de imagenes RGB
	Mat rgbFrame;

	// Bucle de adquisicion de imagenes
	for(;;){
		inputVideo1 >> rgbFrame;
		record << rgbFrame;
		if(waitKey(30)>=0) break;
	}
	return 0;
}

int recordThermalRGBVideo(string destPath){
	// Entrada de imagenes

		VideoCapture inputVideo2("http://root:cuadriga@192.168.0.90/mjpg/2/video.mjpg");
		VideoCapture inputVideo1("http://root:cuadriga@192.168.0.90/mjpg/1/video.mjpg");
		if(!inputVideo1.isOpened() && !inputVideo2.isOpened()){
			cout << "Cameras cannot be opened!";
			return -1;
		}



		Size S = Size((int) inputVideo1.get(CV_CAP_PROP_FRAME_WIDTH) + (int)inputVideo2.get(CV_CAP_PROP_FRAME_WIDTH),
				(int) (inputVideo1.get(CV_CAP_PROP_FRAME_HEIGHT) >= inputVideo1.get(CV_CAP_PROP_FRAME_HEIGHT) ?
						inputVideo1.get(CV_CAP_PROP_FRAME_HEIGHT) : inputVideo2.get(CV_CAP_PROP_FRAME_HEIGHT)));
		VideoWriter record(destPath, CV_FOURCC('M','J','P','G'), 25, S, true);

		if( !record.isOpened()){
			cout << "VideoWriter failed to open!\n";
			return -1;
		}


		// Matriz de imagenes RGB y Thermal
		Mat rgbFrame, thermalFrame;

		// Creacion de ventanas
		//namedWindow("Frames captured (RGB | Thermal)", CV_WINDOW_FREERATIO);
		// Bucle de adquisicion de imagenes
		for(;;){
			inputVideo2 >> thermalFrame;
			inputVideo1 >> rgbFrame;
			// Mostrar las imagenes
			/*
			if(!rgbFrame.empty() && !thermalFrame.empty()){
				imshow("Frames captured (RGB | Thermal)", composeImage(rgbFrame, thermalFrame));
			}
			*/
			record << composeImage(rgbFrame, thermalFrame);
			if(waitKey(30)>=0) break;
		}
		return 0;
}

int recordThermalRGBVideo(string destRGBPath, string destThermalPath){
	// Entrada de imagenes
	VideoCapture inputVideo2("http://root:cuadriga@192.168.0.90/mjpg/2/video.mjpg");
	VideoCapture inputVideo1("http://root:cuadriga@192.168.0.90/mjpg/1/video.mjpg");
	if(!inputVideo1.isOpened() && !inputVideo2.isOpened()){
		cout << "Cameras cannot be opened!";
		return -1;
	}


	// Definir tamaño de las ventanas de grabación y fps
	Size S1 = Size((int) inputVideo1.get(CV_CAP_PROP_FRAME_WIDTH),
			(int)inputVideo1.get(CV_CAP_PROP_FRAME_HEIGHT));
	Size S2 = Size((int) inputVideo2.get(CV_CAP_PROP_FRAME_WIDTH),
				(int)inputVideo2.get(CV_CAP_PROP_FRAME_HEIGHT));
	VideoWriter recordRGB(destRGBPath, CV_FOURCC('M','J','P','G'), 25, S1, true);
	VideoWriter recordThermal(destThermalPath, CV_FOURCC('M','J','P','G'), 25, S2, true);

	if( !recordRGB.isOpened() && !recordRGB.isOpened()){
		cout << "VideoWriter failed to open!\n";
		return -1;
	}

	// Matriz de imagenes RGB y Thermal
	Mat rgbFrame, thermalFrame;

	// Bucle de adquisicion de imagenes
	namedWindow("Frames captured (RGB | Thermal)", CV_WINDOW_FREERATIO);
	cout << "Recording...";
	for(;;){
		inputVideo2.read(thermalFrame);
		inputVideo1.read(rgbFrame);

		imshow("Frames captured (RGB | Thermal)", rgbFrame);
		recordRGB << rgbFrame;
		recordThermal << thermalFrame;

		if(waitKey(1)>=0) break;
	}
	return 0;
}


int frameStep(string sourcePath){
	const string WNAME = "Frames captured (RGB | Thermal)";
	Mat frame;
	VideoCapture inputVideo(sourcePath);

	if(!inputVideo.isOpened()){
				cout << "File cannot be opened!";
				return -1;
	}

	namedWindow(WNAME, CV_WINDOW_FREERATIO);
	// Bucle de muestra de imagenes
	int maxFrames = inputVideo.get(CV_CAP_PROP_FRAME_COUNT);
	string frameText = "frame ", auxText;
	stringstream ss;
	for(int i=0; i < maxFrames; i++){
		ss << frameText << i;
		inputVideo >> frame;
		putText(frame, ss.str(), Point2f(20, 50), FONT_HERSHEY_SIMPLEX,0.5, Scalar(0,0,255,255));

		if(!frame.empty()) imshow(WNAME, frame);
		if(waitKey(100)>0) break;
		ss.str("");
		ss.clear();
	}

	return 0;
}

int frameStep(string sourcePath1, string sourcePath2, int shift){
	const string WNAME = "Frames captured (RGB | Thermal)";
	Mat frame1, frame2;
	VideoCapture inputVideo1(sourcePath1);
	VideoCapture inputVideo2(sourcePath2);

	if(!inputVideo1.isOpened() && !inputVideo2.isOpened()){
				cout << "Files cannot be opened!";
				return -1;
	}

	namedWindow(WNAME, CV_WINDOW_FREERATIO);
	// Bucle de muestra de imagenes
	int maxFrames1 = inputVideo1.get(CV_CAP_PROP_FRAME_COUNT);
	int maxFrames2 = inputVideo2.get(CV_CAP_PROP_FRAME_COUNT);
	int maxFrames = maxFrames1 >= maxFrames2 ? maxFrames2 : maxFrames1;

	string frameText = "frame ", auxText1, auxText2;
	stringstream ss1, ss2;
	int j = 0;
	for(int i=0; i < maxFrames; i++){
		ss1 << frameText << j;
		ss2 << frameText << i;
		inputVideo2 >> frame2;
		if(shift + j == i) {
			inputVideo1 >> frame1;
			putText(frame1, ss1.str(), Point2f(20, 50), FONT_HERSHEY_SIMPLEX,0.5, Scalar(0,0,255,255));
			j++;
		}
		putText(frame2, ss2.str(), Point2f(20, 50), FONT_HERSHEY_SIMPLEX,0.5, Scalar(0,0,255,255));

		if(!frame1.empty() && !frame2.empty()) imshow(WNAME, composeImage(frame1,frame2));
		if(waitKey(100)>0) break;
		ss1.str("");
		ss2.str("");
		ss1.clear();
		ss2.clear();
	}

	return 0;
}

int moveToPTZ(float pan, float tilt, int zoom){

	CURL *curl;
    CURLcode res;

    int responseStatus = -1; // OK
    curl = curl_easy_init();
    stringstream ss;
    ss << "http://root:cuadriga@192.168.0.90/axis-cgi/com/ptz.cgi?rpan=" << pan
    		<< "&rtilt=" << tilt << "&rzoom=" << zoom;
    if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, ss.str().c_str());

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if(res != CURLE_OK){
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
		}else{
			responseStatus = 0;
		}
		/* always cleanup */
		curl_easy_cleanup(curl);
	}
    return responseStatus;
}

int moveToPTContinuous(int panSpeed, int tiltSpeed){

	CURL *curl;
    CURLcode res;

    int responseStatus = -1; // OK
    curl = curl_easy_init();
    stringstream ss;
    ss << "http://root:cuadriga@192.168.0.90/axis-cgi/com/ptz.cgi?continuouspantiltmove=" << panSpeed
    		<< "," << tiltSpeed;
    if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, ss.str().c_str());

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if(res != CURLE_OK){
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
		}else{
			responseStatus = 0;
		}
		/* always cleanup */
		curl_easy_cleanup(curl);
	}
    return responseStatus;
}

int moveToPresetNumber(const int presetNum){
	CURL *curl;
    CURLcode res;

    int responseStatus = -1; // OK
    curl = curl_easy_init();
    stringstream ss;
    ss << "http://root:cuadriga@192.168.0.90/axis-cgi/com/ptz.cgi?gotodevicepreset=" << presetNum;
    if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, ss.str().c_str());

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if(res != CURLE_OK){
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
		}else{
			responseStatus = 0;
		}
		/* always cleanup */
		curl_easy_cleanup(curl);
	}
    return responseStatus;
}

int plotMSERfromVideo(string sourcePath){
	const string WNAME = "Frames captured (RGB | Thermal)";
	Mat frame;
	VideoCapture inputVideo(sourcePath);
	Ptr<MSER> ms = MSER::create(5,200);
	vector< vector<Point> > regions;
    vector<Rect> boxes;

	if(!inputVideo.isOpened()){
				cout << "File cannot be opened!";
				return -1;
	}

	namedWindow(WNAME, CV_WINDOW_FREERATIO);
	// Bucle de muestra de imagenes
	int maxFrames = inputVideo.get(CV_CAP_PROP_FRAME_COUNT);
	string frameText = "frame ", auxText;
	stringstream ss;
	for(int i=0; i < maxFrames; i++){
		ss << frameText << i;
		inputVideo >> frame;
		putText(frame, ss.str(), Point2f(20, 50), FONT_HERSHEY_SIMPLEX,0.5, Scalar(0,0,255,255));

		if(!frame.empty()) {
			ms->detectRegions(frame,regions,boxes);
			for(int j=0; j < boxes.size(); j++){
				rectangle(frame, boxes[j], CV_RGB(0,255,0));
			}
			imshow(WNAME, frame);
		}
		if(waitKey(10)>0) break;
		ss.str("");
		ss.clear();
	}

	return 0;
}


#endif /* OCULUSTI_H_ */

