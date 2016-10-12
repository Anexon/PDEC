/*
 * OculusTI.h
 *
 *  Created on: 27/07/2016
 *      Author: anexon
 */

#ifndef OCULUSTI_H_
#define OCULUSTI_H_

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>

#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

/**
 * Genera una imagen compuesta por la imagen a color y la imagen termica
 * (izquierda y derecha respectivamente)
 */
Mat composeImage(Mat imageOne, Mat imageTwo);

/**
 * Comienza a grabar video de la cámara térmica en el archivo de salida
 * indicado como parámetro. Para terminar pulsar escape.
 */
int recordThermalVideo(string destPath);

/**
 * Comienza a grabar video de la cámara térmica en el archivo de salida
 * indicado como parámetro. Para terminar pulsar escape.
 */
int recordRGBVideo(string destPath);

/**
 * Comienza a grabar video de la cámara térmica y de color en el archivo
 * de salida indicado como parámetro. Para terminar pulsar escape.
 */
int recordThermalRGBVideo(string destPath);

/**
 * Comienza a grabar video de la cámara térmica y de color en los archivos
 * de salida indicados como parámetro. Para terminar pulsar escape.
 */
int recordThermalRGBVideo(string destRGBPath, string destThermalPath);

int frameStep(string sourcePath);

int frameStep(string sourcePath1, string sourcePath2, int shift);

/**
 * Permite realizar movimientos mediante comandos PTZ (Pan Tilt Zoom)
 */
int moveToPTZ(float pan, float tilt, int zoom);

/**
 * Movimiento continuo (positivo pan -> derecha, positivo tilt -> arriba).
 * Para deterner el movimiento usar 0,0
 */
int moveToPTContinuous(int panSpeed, int tiltSpeed);

/**
 * Permite mover la camara a posiciones predefinidas
 */
int moveToPresetNumber(const int presetNum);

/**
 * Muestra, sobre las imagenes de un video grabado los resultados del feature detection MSER
 */
int plotMSERfromVideo(string sourcePath);

/**
 * @brief getMSERs Detecta las regiones MSER
 * @param frame origen
 * @return MSER regions
 */
void getMSERs(Mat frame, float mThreshold, vector< vector<Point> > &regions);

/**
 * @brief plotMSER
 * @param frame
 * @param regions
 * @return Frame origin with ellipses drawed on it
 */
void plotMSER(Mat &outputFrame, vector< vector<Point> > regions);

void getSIFTKps(Mat frame, vector<vector<KeyPoint> > &kPointsVect, vector<vector<Point> > regions);

void drawSIFTKps(Mat frame, vector<vector<KeyPoint> > kPointsVect, Mat &processedFrame);

/**
 * @brief plotFrameNumber
 * @param frame
 * @param frameNum
 * @return Frame with nomber on it
 */
void plotFrameNumber(Mat &frame, int frameNum);

void loadSVM();

#endif /* OCULUSTI_H_ */
