#ifndef WORKERSERIAL_H
#define WORKERSERIAL_H

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

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QMutex>

#include "qextserial/qextserialport.h"

#define  QWT_VECTOR_SIZE     (6)
#define  QWY_DATA_SIZE       (25)

//串口配置类
class serialCfgPreClass
{
public:
    bool flag;
    QextSerialPort *serial = NULL;
    QString name;
    BaudRateType baudRate;
    DataBitsType dataBits;
    ParityType parity;
    StopBitsType stopBits;
    FlowType flow;
    long timeout;
};

//qwt数据类
class qwtDataClass
{
public:
    explicit qwtDataClass()
    {
    }

    QString dataTypeName;
    QVector <double> data;

};

//接受数据类
class allRxDataClass
{
public:
    explicit allRxDataClass()
    {
    }

    QVector <qwtDataClass> curveData;
    int curveSentSize;

};


class SerialWorkerClass : public QObject
{
    Q_OBJECT
public:
    explicit SerialWorkerClass(QObject *parent = 0);

    allRxDataClass allRxData;

    serialCfgPreClass serialCfgPreOne;

    void sentOpenSerialSingleFun();
    void sentCloseSerialSingleFun();

    QTimer  *receiveTimer = NULL;
    QByteArray receiveData;

    void sentRxSingleFun();
    void sentTxSingleFun(QByteArray temp);

signals:
    void rxSingle();
    void txSingle(QByteArray temp);
    void openSerialSinge();
    void closeSerialSinge();
    void wDisplayTxDataSingle(QByteArray temp);
    void wSentSerialState(bool state);

public slots:
    void openSerial();
    void closeSerial();
    void rxData();
    void txData(QByteArray temp);
    void receiveTimerOut();

private:

};

#endif // WORKERSERIAL_H
