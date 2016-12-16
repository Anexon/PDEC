#ifndef MYSVM_H
#define MYSVM_H

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
using namespace std;

class MySVM : public QThread
{   Q_OBJECT
    private:
        SVM svm;
        string datasetFileName;
        VideoCapture inputVideo;
        int datasetSize;
        bool loadDataset(string filename);
    signals:
        void updateProgress(int progress, QString format);
    public:
        MySVM(QObject *parent = 0);
        void train(string datasetFileName);
        void predictRegions(Mat descriptors, vector<bool> &predictions);
        ~MySVM();
    protected:
        void run();
        void msleep(int ms);
};

#endif // MYSVM_H
