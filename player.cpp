#include "player.h"
#include "OculusTI.h"
#include <QTextStream>
#include <QFile>
#include <QRegularExpression>

Player::Player(QObject *parent)
 : QThread(parent)
{
    myMSER = MyMSER(0.8,60,16000,0.8,150);
    mySIFT = MySIFT();
    myFREAK = MyFREAK();
    stop = true;
    descriptorSelected = DESCRIPTORS::SIFT;
}

QTextStream& qStdOut()
{
    static QTextStream ts( stdout );
    return ts;
}


bool Player::loadVideo(String filename) {
    cout << filename;

    capture.open(filename);
    if (capture.isOpened())
    {
        frameRate = (int) capture.get(CV_CAP_PROP_FPS);
        return true;
    }
    else
        return false;
}

void Player::loadGroundTruth(string groundTruthFileName){
    this->groundTruthFileName = groundTruthFileName;
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
    QFile file(groundTruthFileName.data());
    QTextStream groundTruthIn(&file);
    if(file.open(QIODevice::ReadOnly)) {
        cout << "\nAbriendo archivo \n";
        bool dataReady = false;
        while(!groundTruthIn.atEnd() && !dataReady) {
            QString line = groundTruthIn.readLine();
            QStringList fields = line.split("%");
            if(fields.length() < 2){
                dataReady = true;
                cout << "Imágenes: " << line.toStdString() << "\n";
            }
        }
    }

    int delay = (1000/frameRate);
    int frameNum = 0;


    // Read frame
    if (!capture.read(frame))
    {
        stop = true;
    }
    // MainLoop
    while(!stop){

        // Assures not dividing by zero
        if(subsampleRate == 0){
            subsampleRate += 1;
        } else if(!subsampleRateCheck){
            subsampleRate = 1;
        }

        if( frameNum % subsampleRate == 0 || frameNum == 0){
            vector<vector<Point> >  regions;
            Mat                     descriptors;
            vector< KeyPoint>       keyPoints;
            vector< Mat>            keyPointFrames;
            vector<bool>            predictions;

            // Start up processed Frame
            resize(frame,frameResized, frameSize);

            // Define margin of 5% for every sides
            Rect myROI = Rect(frameSize.width*0.05, frameSize.height*0.05, (int)frameSize.width*0.9, (int)frameSize.height*0.9);
            frameResized(myROI).copyTo(frameCropped);
            frameCropped.copyTo(processedFrame);


            // Get MSER Regions
            cout << "\nExtracting MSERs...";
            myMSER.getMSERs(processedFrame, regions, keyPoints, keyPointFrames);
            // Show feature detector if checkButton is checked
            if(showFeatureDetector){
                myMSER.plotMSER(frameCropped, regions, processedFrame);
            }

            // Get Descritors from SIFT Descriptor and draw keypoints into processed frame
            cout << "\nExtracting descriptors...";
            getDescriptors(keyPoints, keyPointFrames, descriptors);

            // Show feature descriptor if checkButton is checked
            if(showFeatureDescriptor){
                drawDescriptors(processedFrame, keyPoints, processedFrame);
            }

            if(showGroundTruth){
                QString line = groundTruthIn.readLine();
                cout << "\nImagen: " << line.toStdString();
                QRegularExpression re("(\\(|\\)\\ )");

                QStringList data = line.split(re);
                for(int i=1; i<data.length(); i++){
                    cout << "Rect " << i << ": " << data[i].toStdString() << "\n";
                }

                //drawGroundTruth(processedFrame, groundTruthBoxes);
            }

            // Predict detected regions
            cout << "Predicting detected regions...\n";
            mySVM->predictRegions(descriptors, predictions);

            plotPredictedRegions(processedFrame, regions, predictions, processedFrame);
            predictions.clear();
            //plotPredictedRegions(processedFrame, regions, predictions, processedFrame);
            // Draw frame number
            //plotFrameNumber(processedFrame, frameNum, processedFrame);

            // Convert to QImage to plot into the UI Window
            if (processedFrame.channels()== 3){
                cv::cvtColor(processedFrame, processedFrame, CV_BGR2RGB);
                img = QImage((const unsigned char*)(processedFrame.data),
                                  processedFrame.cols,processedFrame.rows,QImage::Format_RGB888);
            } else {
                img = QImage((const unsigned char*)(processedFrame.data),
                             processedFrame.cols,processedFrame.rows,QImage::Format_Indexed8);
            }

            // Clear processed frame for the next step
            processedFrame.release();
            frameCropped.release();
            frameResized.release();
        }

        // Notify to UI proccessed img
        emit processedImage(img);
        frameNum++;
        this->msleep(delay/subsampleRate);

        // Read frame
        if (!capture.read(frame))
        {
            stop = true;
        }
    }  
    processedFrame.release();
}

void Player::plotPredictedRegions(Mat frame, vector<vector<Point> > regions, vector<bool> predictions, Mat &processedFrame){
    if(!frame.empty()) {
        cout << "Drawing predicted regions...\n";
        for(unsigned j=0; j < regions.size(); j++){
            for(unsigned i=0; i < regions[j].size(); i++){
                int color = 0;
                if(predictions[j]) color = 255;
                Vec3b intensity;
                intensity.val[0] = 0;
                intensity.val[1] = 0;
                intensity.val[2] = color;
                //intensit.val[0] = color
                frame.at<Vec3b>(regions[j][i]) = intensity;
            }
        }
    }
}

void Player::getDescriptors(vector<KeyPoint> keyPoints, vector<Mat> keyPointFrames, Mat &descriptors){
    switch (descriptorSelected) {
        case SIFT:
            mySIFT.getDescriptors(keyPoints, keyPointFrames, descriptors);
            break;
        case FREAK:
            myFREAK.getDescriptors(keyPoints, keyPointFrames, descriptors);
            break;
        default:
            break;
    }
}

void Player::drawDescriptors(Mat processedFrame, vector<KeyPoint> keyPoints, Mat &procFrame){
    switch (descriptorSelected) {
        case SIFT:
            mySIFT.drawKeyPoints(processedFrame, keyPoints, procFrame);
            break;
        case FREAK:
            myFREAK.drawKeyPoints(processedFrame, keyPoints, procFrame);
            break;
        default:
            break;
    }
}

void Player::setSVM(MySVM* mySVM){
    this->mySVM = mySVM;
}

void Player::setDescriptor(DESCRIPTORS descriptor){
    this->descriptorSelected = descriptor;
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
