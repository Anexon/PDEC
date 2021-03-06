#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <player.h>
#include <mysvm.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //Display video frame in player UI
    void updatePlayerUI(QImage img);

    // Update progressBar
    void updateProgressBar(int completionPercent, QString format);

    void on_exitButton_clicked();

    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_checkBox_clicked();

    void on_descriptor_combobox_activated(const QString &arg1);

    void on_descriptor_show_clicked();

    void on_subsampleRateCheckBox_clicked(bool checked);

    void on_subsampleRate_valueChanged(int arg1);

    void on_actionTrain_SVM_triggered();

    void on_actionLoad_GroundTruth_triggered();

    void on_descriptor_combobox_currentIndexChanged(int index);

    void on_showGroundTruthCheck_clicked();

private:
    Ui::MainWindow *ui;
    Player* myPlayer;
    MySVM* mySVM;
};

#endif // MAINWINDOW_H
