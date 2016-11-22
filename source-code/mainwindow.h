#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
*********************************************************************************************************
*
*	@ProjectName : curve-qwt-com-thread
*	@Description :
*	@Version     : V1.0
*	@Description :
*
*	....................Update....................
*	@Version  @Date      @Design   @Description
*    V1.0    2016.11.23  Aslm       demo
*
* @ME:sillyman2008@outlook.com
*********************************************************************************************************/

#include <QMainWindow>
#include <QTimer>

#include "qextserial/qextserialport.h"
#include "myserial/workerserial.h"
#include "pidform.h"
#include "aboutdialog.h"

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
    //about com
    void displayRxData(QByteArray temp);

    void sentTextTxDate();

    //about ui
    void quit();

    void getSerialState(bool state);

    void on_sentMsgPushButton_clicked();

    void on_repeateSentCheckBox_clicked();

    void on_repeatDelaySpinBox_valueChanged(int arg1);

    void on_clearTextPushButton_clicked();

    void on_openSerialPushButton_clicked();

    void on_closeSerialPushButton_clicked();

    void on_saveCfgPushButton_clicked();

    void on_actionCurveView_triggered();

    void on_actionAbout_ass_triggered();

private:
    //about com
    QTimer *repeatTimer= NULL;
    unsigned int repeatSentDelay;
    QThread serialThreadOne;
    SerialWorkerClass serialWorkerOne;

    //about ui
    Ui::MainWindow *ui;
    pidForm myPidForm;
    AboutDialog aboutdlg;
};

#endif // MAINWINDOW_H
