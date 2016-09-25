#include "player.h"
#include "OculusTI.h"

Player::Player(QObject *parent)
 : QThread(parent)
{
    stop = true;
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
        if (!capture.read(frame))
        {
            stop = true;
        }
        processedFrame.release();
        detectAndPlotMSER(frame,frameNum).copyTo(processedFrame);

        if (frame.channels()== 3){
            cv::cvtColor(processedFrame, processedFrame, CV_BGR2RGB);
            img = QImage((const unsigned char*)(processedFrame.data),
                              processedFrame.cols,processedFrame.rows,QImage::Format_RGB888);
        }
        else
        {
            img = QImage((const unsigned char*)(processedFrame.data),
                                 processedFrame.cols,processedFrame.rows,QImage::Format_Indexed8);
        }

        emit processedImage(img);
        frameNum++;
        this->msleep(delay);
    }
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
