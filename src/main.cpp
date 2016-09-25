#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>

#include "OculusTI.h"

using namespace cv;
using namespace std;

void *record(void *threadid){
	recordThermalRGBVideo("rgbVideo5.avi", "thermalVideo5.avi");
	pthread_exit(NULL);
}

void *move(void *threadid){
	int k = 1;
	for(int i = 1; i <= 4; i++){
		moveToPTContinuous(10*k,0);
		sleep(20);
		k = -1*k;
	}
	moveToPTContinuous(0,0);
	pthread_exit(NULL);
}

int main(int argc, char ** argv){

	int result = 0;
	/*
	pthread_t threads[2];
	int rc1 = pthread_create(&threads[0], NULL, record, (void *)0);
	int rc2 = pthread_create(&threads[1], NULL, move, (void *)1);
	if( rc1 || rc2){
		cout << "Error: Unable to create thread, " << rc1 << ", " << rc2;
		exit(-1);
	}
	pthread_exit(NULL);
	*/
	//result = recordThermalRGBVideo("rgbVideo4.avi", "thermalVideo4.avi");
	//result = frameStep("thermalVideo5.avi");
	result = plotMSERfromVideo("assets/video/thermalVideo5.avi");
	//result = moveToPTZ(10,0,0);
	//result = moveToPresetNumber(1);

	return result;
}
