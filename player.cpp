#include "player.h"
#include "OculusTI.h"
#include <QTextStream>

Player::Player(QObject *parent)
 : QThread(parent)
{
    stop = true;
}

QTextStream& qStdOut()
{
    static QTextStream ts( stdout );
    return ts;
}


bool Player::loadVideo(String filename) {
    capture.open(filename);
    if (capture.isOpened())
    {
        frameRate = (int) capture.get(CV_CAP_PROP_FPS);
        return true;
    }
    else
        return false;
}

void Player::Play()
{
    if (!isRunning()) {
        if (isStopped()){
            stop = false;
        }
        start(LowPriority);
    }
}

void Player::run()
{
    int delay = (1000/frameRate);
    int frameNum = 0;
    while(!stop){
        // Read frame
        if (!capture.read(frame))
        {
            stop = true;
        }

        // Assures not dividing by zero
        if(subsampleRate == 0){
            subsampleRate += 1;
        } else if(!subsampleRateCheck){
            subsampleRate = 1;
        }

        if( frameNum % subsampleRate == 0 || frameNum == 0){
            vector<vector<Point> >      regions;
            vector<vector<KeyPoint> >   kPointsVect;

            // Start up precessed Frame
            resize(frame,frameResized, frameSize);
            // Define margin of 5% for every sides
            Rect myROI = Rect(frameSize.width*0.05, frameSize.height*0.05, (int)frameSize.width*0.9, (int)frameSize.height*0.9);
            frameResized(myROI).copyTo(frameCropped);
            frameCropped.copyTo(processedFrame);

            // Show feature detector if checkButton is checked
            if(showFeatureDetector){
                // Get MSER Regions and plot them into processed frame
                float myThreshold = 60;
                getMSERs(frameCropped, myThreshold, regions);
                plotMSER(processedFrame, regions);
            }

            // Show feature descriptor if checkButton is checked
            if(showFeatureDescriptor){
                // Get Descritors from SIFT Descriptor and draw keypoints into processed frame
                getSIFTKps(frameCropped, kPointsVect, regions);
                drawSIFTKps(processedFrame, kPointsVect, processedFrame);
            }

            // Draw frame number
            plotFrameNumber(processedFrame, frameNum);
            if (processedFrame.channels()== 3){
                cv::cvtColor(processedFrame, processedFrame, CV_BGR2RGB);
                img = QImage((const unsigned char*)(processedFrame.data),
                                  processedFrame.cols,processedFrame.rows,QImage::Format_RGB888);
            }
            else
            {
                img = QImage((const unsigned char*)(processedFrame.data),
                             processedFrame.cols,processedFrame.rows,QImage::Format_Indexed8);
            }
            // Clear processed frame for the next step
            processedFrame.release();
        }

        // Notify to UI proccessed img
        emit processedImage(img);
        frameNum++;
        this->msleep(delay/subsampleRate);
    }  
    //processedFrame.release();
}

Player::~Player()
{
    mutex.lock();
    stop = true;
    capture.release();
    condition.wakeOne();
    mutex.unlock();
    wait();
}
void Player::Stop()
{
    stop = true;
}
void Player::msleep(int ms){
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}
bool Player::isStopped() const{
    return this->stop;
}
