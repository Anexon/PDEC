#include "mysvm.h"

MySVM::MySVM(QObject *parent) : QThread(parent)
{

}

bool MySVM::loadDataset(String filename){
    capture.open(filename);
    if (capture.isOpened())
    {
        // Dump video into vector to perform labeling of dataset
        Mat frame;
        while(!capture.read(frame)){
            dataset.push_back(frame);
        }
        return true;
    }
    else
        return false;
}

void MySVM::execute(String datasetFileName){
    this->datasetFileName = datasetFileName;
    if(!isRunning()){
        start(LowPriority);
    }
}

void MySVM::run(){
    if(loadDataset(datasetFileName)){
        //kmeans(dataset,2,)
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
