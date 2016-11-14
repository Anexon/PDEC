#ifndef PLAYER_H
#define PLAYER_H
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QTextStream>
#include <mymser.h>
#include <mysift.h>

using namespace cv;
class Player : public QThread
{    Q_OBJECT
 private:
     bool stop;
     QMutex mutex;
     QWaitCondition condition;
     Mat frame;
     Mat frameResized;
     Mat frameCropped;
     int frameRate;
     VideoCapture capture;
     Mat processedFrame;
     QImage img;

     MyMSER myMSER;
     MySIFT mySIFT;
 signals:
 //Signal to output frame to be displayed
      void processedImage(const QImage &image);
 protected:
     void run();
     void msleep(int ms);
 public:
    //Constructor
    Player(QObject *parent = 0);
    //Destructor
    ~Player();
    // Variables for drawing content
    Size frameSize = Size(320,200);
    bool showFeatureDetector = true;
    bool showFeatureDescriptor = true;
    int subsampleRate = 0;
    bool subsampleRateCheck = false;
    //Load a video from memory
    bool loadVideo(String filename);
    //Play the video
    void Play();
    //Stop the video
    void Stop();
    //check if the player has been stopped
    bool isStopped() const;
};
#endif // VIDEOPLAYER_H
