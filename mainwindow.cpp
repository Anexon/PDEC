#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    myPlayer = new Player();
    mySVM = new MySVM();
    myPlayer->setSVM(mySVM);

    // Define connections between entities
    QObject::connect(myPlayer, SIGNAL(processedImage(QImage)),
                              this, SLOT(updatePlayerUI(QImage)));
    QObject::connect(mySVM, SIGNAL(updateProgress(int, QString)), this, SLOT(updateProgressBar(int, QString)));
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
}

MainWindow::~MainWindow()
{
    delete myPlayer;
    delete ui;
}

void MainWindow::updatePlayerUI(QImage img)
{
    if (!img.isNull())
    {
        ui->label->setAlignment(Qt::AlignCenter);
        ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(),
                                           Qt::KeepAspectRatio, Qt::FastTransformation));
    }
}

void MainWindow::updateProgressBar(int completionPercent, QString format)
{

    ui->progressBar->setFormat(format);
    if(completionPercent < 100){
        ui->progressBar->setValue(completionPercent);
        ui->progressBar->show();
    } else {
        ui->progressBar->setValue(100);
        //struct timespec ts = { 1000 / 1000, (1000 % 1000) * 1000 * 1000 };
        //nanosleep(&ts, NULL);
        //ui->progressBar->hide();
    }
}

void MainWindow::on_exitButton_clicked()
{
    if (myPlayer->isStopped())
    {
        myPlayer->Play();
        ui->exitButton->setText(tr("Pause"));
    }else
    {
        myPlayer->Stop();
        ui->exitButton->setText(tr("Resume"));
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("Text Files (*.avi *.mpg *.mp4)"));

    if (!fileName.isEmpty()) {
        if (!myPlayer->loadVideo(fileName.toLocal8Bit().data())) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        } else if (myPlayer->isStopped())
        {
            myPlayer->Play();
            ui->exitButton->setEnabled(true);
        }else
        {
            myPlayer->Stop();
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}

void MainWindow::on_checkBox_clicked()
{
    if(myPlayer->showFeatureDetector){
        myPlayer->showFeatureDetector = false;
    } else {
        myPlayer->showFeatureDetector = true;
    }
}

void MainWindow::on_descriptor_show_clicked()
{
    if(myPlayer->showFeatureDescriptor){
        myPlayer->showFeatureDescriptor = false;
    } else {
        myPlayer->showFeatureDescriptor = true;
    }
}

void MainWindow::on_descriptor_combobox_activated(const QString &arg1)
{

}

void MainWindow::on_subsampleRateCheckBox_clicked(bool checked)
{
    myPlayer->subsampleRateCheck = checked;
    if(checked){
        myPlayer->subsampleRate = ui->subsampleRate->value();
    } else {
        myPlayer->subsampleRate = 0;
    }
}

void MainWindow::on_subsampleRate_valueChanged(int arg1)
{
    myPlayer->subsampleRate = arg1;
}

void MainWindow::on_actionTrain_SVM_triggered()
{
    ui->progressBar->setValue(0);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("Text Files (*.avi *.mpg *.mp4)"));
    mySVM->train(fileName.toLocal8Bit().data());
}

void MainWindow::on_descriptor_combobox_currentIndexChanged(int index)
{
    // Descriptor changed
    switch (index) {
    case 0:
        myPlayer->setDescriptor(Player::DESCRIPTORS::SIFT);
        cout << "SIFT Descriptor Selected";
        break;
    case 1:
        myPlayer->setDescriptor(Player::DESCRIPTORS::FREAK);
        cout << "FREAK Descriptor Selected";
        break;
    default:
        break;
    }
}
