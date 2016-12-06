#include "mysvm.h"

MySVM::MySVM(QObject *parent) : QThread(parent)
{

}

bool MySVM::loadDataset(string filename){
    inputVideo.open(filename);
    if (inputVideo.isOpened())
    {
        datasetSize = inputVideo.get(CV_CAP_PROP_FRAME_COUNT);
        return true;
    } else {
        return false;
    }
}

void MySVM::train(string datasetFileName){
    this->datasetFileName = datasetFileName;
    if(!isRunning()){
        start(LowPriority);
    }
}

void MySVM::run(){
    // Init MSER and SIFT objects
    MyMSER myMSER = MyMSER(4,60,600,0.25,150);
    MySIFT mySIFT = MySIFT();

    if(loadDataset(datasetFileName)){
        Mat frame;
        // Global Characteristics Vector
        Mat globalDescriptorVector;

        // Update ProgressBar
        emit updateProgress(10, "Training SVM... ("+QString::number(10)+"%)");

        for(int I = 0; I < datasetSize; I++){
            cout << "Aqui \n";
            inputVideo >> frame;
            // Get MSER regions
            vector<vector<Point> > regions;
            myMSER.getMSERs(frame, regions);

            // Get Description Vector (SIFT)
            vector<vector<KeyPoint> >   kPointsVect;
            vector<Mat> descriptors;
            mySIFT.getSIFTKps(frame, kPointsVect, descriptors, regions);

            // Save al extracted descriptors
            for(unsigned J = 0; J < descriptors.size(); J++){
                for(unsigned K = 0; K < descriptors[J].rows; K++){
                    globalDescriptorVector.push_back(descriptors[J].row(K));
                }
            }
            regions.clear();
            descriptors.clear();
            kPointsVect.clear();
        }
        cout << "Global Characteristics Vectors Size -> " << globalDescriptorVector.size() << "\n";

        // Update ProgressBar
        emit updateProgress(60, "Training SVM... ("+QString::number(60)+"%)");

        // Descriptors storage
        FileStorage fs("descriptors.txt", FileStorage::WRITE);
        write(fs, "descriptors", globalDescriptorVector);
        fs.release();

        // Generate labeled matrix
        Mat labels;
        kmeans(globalDescriptorVector,2,labels, TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0),3, KMEANS_RANDOM_CENTERS);

        // Labels storage
        FileStorage fl("labels.txt", FileStorage::WRITE);
        write(fl, "labels", labels);
        fl.release();

        // Set up SVM's parameters
        CvSVMParams params;
        params.svm_type    = CvSVM::C_SVC;
        params.kernel_type = CvSVM::LINEAR;
        params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

        // Train the SVM
        this->svm.train(globalDescriptorVector, labels, Mat(), Mat(), params);

        // Update ProgressBar
        emit updateProgress(100, "SVM trained successfully!");
    }
}

MySVM::~MySVM()
{
    //mutex.lock();
    //stop = true;
    //capture.release();
    //condition.wakeOne();
    //mutex.unlock();
    wait();
}
