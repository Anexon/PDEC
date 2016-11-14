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
    QObject::connect(myPlayer, SIGNAL(processedImage(QImage)),
                              this, SLOT(updatePlayerUI(QImage)));
    ui->setupUi(this);
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("Text Files (*.avi *.mpg *.mp4)"));
}
