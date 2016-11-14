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

using namespace cv;

class MySVM : public QThread
{   Q_OBJECT
    private:
        String datasetFileName;
        vector<Mat> dataset;
        VideoCapture capture;

        bool loadDataset(String filename);
    public:
        MySVM(QObject *parent = 0);
        void execute(String datasetFileName);
        ~MySVM();
    protected:
        void run();
        void msleep(int ms);
};

#endif // MYSVM_H
