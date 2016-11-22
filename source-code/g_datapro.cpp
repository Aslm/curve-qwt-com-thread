#include "g_datapro.h"
#include <QDebug>
#include "g_define.h"

G_dataProClass G_dataPro;

G_dataProClass::G_dataProClass(QObject *parent) : QObject(parent)
{
    qwtAllCurvePre.pidData.resize(PID_CURVE_SIZE);

    //default
    qwtAllCurvePre.pidFromX_DisplaySize = 100;
    qwtAllCurvePre.pidFromGrep_x = 1;
}


bool G_dataProClass::qwtFromDataPro(allRxDataClass *allRxData)
{
    if(G_dataPro.qwtAllCurvePre.pidDataProFlag == false)
    {
        return false;
    }

    if(allRxData == NULL)
    {
        qDebug() << "\033[35m warring : qwtFromDataPro *allRxData wrong \033[0m";
        return false;
    }

    if(allRxData->curveData.size() > qwtAllCurvePre.pidData.size())
    {
        qDebug() << "\033[35m warring : qwtAllCurvePre.pidData.size() too few \033[0m";
        return false;
    }

    bool proFlag = false;

#if 0
    qDebug() << "pidDataX.size" << qwtAllCurvePre.pidDataX.size() << "pidDataXmaxSize"
             << qwtAllCurvePre.pidFromX_DisplaySize;
#endif
    if(qwtAllCurvePre.pidDataX.size() >= qwtAllCurvePre.pidFromX_DisplaySize)
    {
        for(int i = 0; i < allRxData->curveSentSize; ++ i)
        {
            qwtAllCurvePre.pidDataX.pop_front();
        }
        proFlag = true;
    }

    for(int i = 0;i < allRxData->curveSentSize ;++ i)
    {
        qwtAllCurvePre.pidDataX.append(qwtAllCurvePre.pidFromGrep_x * (i + 1)
                                       + qwtAllCurvePre.pidDataXtemp);
    }
    qwtAllCurvePre.pidDataXtemp = qwtAllCurvePre.pidDataX.back();

#if 0
    for(int i = 0;i < allRxData->pidSentSize ;++ i)
    {
        qDebug() << "pidDataX " << i << qwtAllCurvePre.pidDataX.at(i);
    }
#endif

    if(proFlag == true)
    {
        for(int i = 0;i < allRxData->curveData.size();++ i)
        {
            for(int j = 0; j < allRxData->curveSentSize; ++ j)
            {
                qwtAllCurvePre.pidData[i].Y.pop_front();
            }
        }
    }
    for(int i = 0;i < allRxData->curveData.size();++ i)
    {
        for(int j = 0;j < allRxData->curveSentSize;++ j)
        {
            qwtAllCurvePre.pidData[i].Y.append(allRxData->curveData.at(i).data.at(j));
        }
    }
    qwtAllCurvePre.pidCurveDataSize = allRxData->curveData.size();

    qRegisterMetaType<class qwtAllCurvePreClass>("class qwtAllCurvePreClass");
    emit qwtPidFromDataSignal(&qwtAllCurvePre);

    return true;
}




