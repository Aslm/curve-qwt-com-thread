
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

#include "workerserial.h"

#include <qwt_math.h>
#include <QObject>

#include "g_datapro.h"

SerialWorkerClass::SerialWorkerClass(QObject *parent) : QObject(parent)
{
    //给定初始容量，用于后续的操作
    allRxData.curveData.resize(QWT_VECTOR_SIZE);
    for(int i = 0; i < allRxData.curveData.size(); ++ i)
    {
        allRxData.curveData[i].data.resize(QWY_DATA_SIZE);
    }

    //数据指定数据类型，用于后续数据处理判定
    for(uint i = 0;i < QWT_VECTOR_SIZE;i++)
    {
        allRxData.curveData[i].dataTypeName = "curve";
    }

    QObject::connect(this, SIGNAL(txSingle(QByteArray)),
                     this, SLOT(txData(QByteArray)));
    QObject::connect(this, SIGNAL(openSerialSinge()),
                     this, SLOT(openSerial()));
    QObject::connect(this, SIGNAL(closeSerialSinge()),
                     this, SLOT(closeSerial()));

}

void SerialWorkerClass::sentRxSingleFun()
{
    emit rxSingle();
}

void SerialWorkerClass::sentTxSingleFun(QByteArray temp)
{
#if 0
    qDebug() << "txSingle(temp)";
#endif

    emit txSingle(temp);
}

void SerialWorkerClass::sentOpenSerialSingleFun()
{
    emit openSerialSinge();
}

void SerialWorkerClass::sentCloseSerialSingleFun()
{
    emit closeSerialSinge();
}

void SerialWorkerClass::openSerial()
{
    if(serialCfgPreOne.serial == NULL)
    {
#ifdef Q_OS_LINUX
        serialCfgPreOne.serial = new QextSerialPort("/dev/" + serialCfgPreOne.name);
#elif defined (Q_OS_WIN)
        serialCfgPreOne.serial = new QextSerialPort(serialCfgPreOne.name);
#endif
        connect(serialCfgPreOne.serial, SIGNAL(readyRead()), this, SLOT(rxData()));
    }

    serialCfgPreOne.serial->setBaudRate(serialCfgPreOne.baudRate);
    serialCfgPreOne.serial->setDataBits(serialCfgPreOne.dataBits);
    serialCfgPreOne.serial->setParity(serialCfgPreOne.parity);
    serialCfgPreOne.serial->setStopBits(serialCfgPreOne.stopBits);
    //设置数据流控制
    serialCfgPreOne.serial->setFlowControl(serialCfgPreOne.flow);
    //设置延时
    serialCfgPreOne.serial->setTimeout(serialCfgPreOne.timeout);

    if(serialCfgPreOne.serial->open(QIODevice::ReadWrite))
    {
#if 1
        qDebug() << P_GREEN "open " << serialCfgPreOne.name << "succes"
                 << "Thread ID : " << QThread::currentThreadId() << P_NONE;
#endif
        emit wSentSerialState(true);
        serialCfgPreOne.flag = true;
    }
    else
    {
#if 1
        qDebug() << P_RED "err : open " << serialCfgPreOne.name << "fail" << P_NONE;
#endif
        delete serialCfgPreOne.serial;
        serialCfgPreOne.serial = NULL;

        emit wSentSerialState(false);
        serialCfgPreOne.flag = false;
        qDebug() << serialCfgPreOne.flag;
        return;
    }
}

void SerialWorkerClass::closeSerial()
{
    if(serialCfgPreOne.serial != NULL)
    {
        serialCfgPreOne.serial->close();
        delete serialCfgPreOne.serial;
    }

    serialCfgPreOne.serial = NULL;

#if 1
        qDebug() << P_GREEN "close " << serialCfgPreOne.name << "succes" << P_NONE;
#endif
}

void SerialWorkerClass::rxData()
{
    if(serialCfgPreOne.serial == NULL)
    {
#if 1
        qDebug() << P_RED "err : serialCfgPreOne.serial == NULL "
                 << serialCfgPreOne.name << P_NONE;
#endif
        return;
    }

#if 1
    static bool p_flag = false;
    if(p_flag == false)
    {
        p_flag = true;
        qDebug() << P_GREEN << "WorkerSerial::rxData Thread ID : "<< QThread::currentThreadId()
                 <<"Seial name "<<serialCfgPreOne.name << P_NONE;
    }
#endif

    QByteArray temp = serialCfgPreOne.serial->readAll();


#if 0
    qDebug() << P_GREEN << "receive data : " << temp << P_NONE;
#endif

    if(receiveTimer == NULL)
    {
        receiveTimer = new QTimer;
        connect(receiveTimer, SIGNAL(timeout()), this, SLOT(receiveTimerOut()));
        receiveTimer->setSingleShot(true);
    }

    if(receiveTimer->isActive())
    {
        //       receiveTimer->stop();
    }
    else
    {
    }
    receiveTimer->start(5);

    for(int i = 0; i < temp.size(); ++i)
    {
        receiveData.push_back(temp.at(i));
    }

#if 0
    //**************数据处理 test***************
    uint num = 0;
    static double x = 0;
    allRxData.pidData[0].data[0] = (qSin(x));
    allRxData.pidData[1].data[0] = (qCos(x));
    allRxData.pidData[2].data[0] = 0;
    x+=(M_PI / 10.0);
    allRxData.pidData[0].data[1] = (qSin(x));
    allRxData.pidData[1].data[1] = (qCos(x));
    allRxData.pidData[2].data[1] = 0;
    x+=(M_PI / 10.0);
    num++;
    allRxData.pidSentSize = 2;
    G_dataPro.qwtFromDataPro(&allRxData);
    //**************数据处理 test end***************
#endif

}

void SerialWorkerClass::txData(QByteArray temp)
{
#if 0
    qDebug() << P_GREEN << "WorkerSerial::txSerial Thread ID : " << QThread::currentThreadId()
             << "Seial name"<<serialPoint->portName() << P_GREEN ;
#endif

#if 0
    qDebug() << P_GREEN << "txData data" << temp << P_GREEN ;
#endif

    if(serialCfgPreOne.serial == NULL)
    {
#if 1
        qDebug() << P_RED "err : serialCfgPreOne.serial == NULL "
                 << serialCfgPreOne.name << P_NONE;
#endif
        return;
    }
    //发送数据写入串口
    serialCfgPreOne.serial->write(temp);
}

int uint8x2_Turn_int16(unsigned char data_h,unsigned char data_l)
{
    static int temp = 0;

    temp = 0;

    if(data_h & 0x80)
    {
        temp = - (unsigned int)(((unsigned int)(data_h & 0x7F) << 8) + data_l);
    }
    else
        temp = + (unsigned int)(((unsigned int)(data_h & 0x7F) << 8) + data_l);

    return temp;
}

void SerialWorkerClass::receiveTimerOut()
{
#if 1
    static bool p_flag = false;
    if(p_flag == false)
    {
        p_flag = true;
        qDebug() << P_GREEN << "WorkerSerial::receiveTimerOut Thread ID : "
                 << QThread::currentThreadId()
                 << "Seial name " << serialCfgPreOne.name << P_NONE;
    }
#endif

    if(serialCfgPreOne.serial == NULL)
    {
#if 1
        qDebug() << P_RED "err : serialCfgPreOne.serial == NULL "
                 << serialCfgPreOne.name << P_NONE;
#endif
        return;
    }

#if 0
    qDebug() << "receive data " << receiveData.size() << receiveData;
#endif

    //接受数据在窗口显示 信号
    emit wDisplayTxDataSingle(receiveData);

#define C_HEND_END_SIZE       4
#define C_DATA_SIZE           12
#define C_CLC_FLAG_SIZE       1

    QVector<char> c_hend;
    QVector<char> c_end;
    unsigned char c_clcFlagL = 0;
    int ageSize = 0;
    int ageDataSize = C_DATA_SIZE;
    c_hend.push_back('{');
    c_hend.push_back('C');
    c_end.push_back('E');
    c_end.push_back('}');


    int receiveFlag = 0;
    int num_hend = 0,num_end = 0;
    if(receiveData.size() >= C_HEND_END_SIZE)
    {
        for(int i = 0; i < receiveData.size(); ++i)
        {
            if((receiveData.size() - i >= c_hend.size()) &&
                    receiveData.at(i) == c_hend.at(0) &&
                    receiveData.at(i + 1) == c_hend.at(1))
            {
                num_hend = i;
                receiveFlag ++;
            }

            if(receiveFlag == 1 &&
                    (receiveData.size() - i >= c_end.size()) &&
                    receiveData.at(i) == c_end.at(0) &&
                    receiveData.at(i + 1) == c_end.at(1))
            {
                receiveFlag ++;
                num_end = i + 1;
            }
        }

        ageSize = num_end - num_hend + 1;

        if((receiveFlag == 2) &&
                ageSize == (C_HEND_END_SIZE + ageDataSize + C_CLC_FLAG_SIZE))
        {
            for(int i = 0; i < ageDataSize; ++i)
            {
                c_clcFlagL += receiveData.at(c_hend.size() + i);
            }

            if(((c_clcFlagL & 0xFF) + (receiveData.at(c_hend.size() + ageDataSize) & 0xFF) != 0xFF))
            {
#if 1

                qDebug() << P_RED << "err : agreement clc wrong! "
                         << P_NONE;
                qDebug("clc : %x | %x",
                       (c_clcFlagL) & 0xFF,
                       receiveData.at(c_hend.size() + ageDataSize) & 0xFF);
#endif
                goto ERR;
            }
        }
        else
        {
#if 1
            qDebug() << P_RED << "err : agreement data size wrong! size :" << ageSize
                     << " num_end : " << num_end << " num_hend :" << num_hend << P_NONE;
#endif
            goto ERR;
        }

        if(receiveFlag == 2)
        {
#if 0
            qDebug() << "agreement data " << receiveData.mid(num_hend , num_end + 1);
#endif
        }
        else
        {
#if 1
            static int err = 0;
            qDebug() << P_RED << "err : agreement unknow err! receiveFlag :" << receiveFlag << "\r\n"
                     << "receive data :" << receiveData.toHex() << "\r\n"
                     << "size :" << receiveData.size() << "\r\n"
                     << "err count" << err++ << P_NONE;
#endif
            goto ERR;
        }

    }
    else
    {
#if 1
        qDebug() << P_RED "err : agreement receiveData.size() to few "
                 << serialCfgPreOne.name << P_NONE;
#endif
        goto ERR;
    }

#if 1

    allRxData.curveData[0].data[0] =
            (double)uint8x2_Turn_int16(receiveData.at(num_hend + 2),receiveData.at(num_hend + 3))  / 1;
    allRxData.curveData[1].data[0] =
            (double)uint8x2_Turn_int16(receiveData.at(num_hend + 4),receiveData.at(num_hend + 5))  / 1;
    allRxData.curveData[2].data[0] =
            (double)uint8x2_Turn_int16(receiveData.at(num_hend + 6),receiveData.at(num_hend + 7))  / 1;
    allRxData.curveData[3].data[0] =
            (double)uint8x2_Turn_int16(receiveData.at(num_hend + 8),receiveData.at(num_hend + 9))  / 1;
    allRxData.curveData[4].data[0] =
            (double)uint8x2_Turn_int16(receiveData.at(num_hend + 10),receiveData.at(num_hend + 11)) / 1;
    allRxData.curveData[5].data[0] =
            (double)uint8x2_Turn_int16(receiveData.at(num_hend + 12),receiveData.at(num_hend + 13)) / 1;

#if 0
    qDebug() << "turn data "
             << allRxData.pidData[0].data[0]
            << allRxData.pidData[1].data[0]
            << allRxData.pidData[2].data[0];
#endif
    allRxData.curveSentSize = 1;
    G_dataPro.qwtFromDataPro(&allRxData);

#endif

ERR:
receiveData.clear();
return;
}

