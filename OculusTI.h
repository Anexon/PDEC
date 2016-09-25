/*
 * OculusTI.h
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

Mat detectAndPlotMSER(Mat frame, int frameNum);

#endif /* OCULUSTI_H_ */
