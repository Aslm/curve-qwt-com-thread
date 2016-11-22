#ifndef G_DATAPRO_H
#define G_DATAPRO_H

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
#include <QVector>

#include "g_define.h"
#include "pidform.h"
#include "myqwtplot/myqwtplot.h"
#include "myserial/workerserial.h"

#define PID_CURVE_SIZE  (6)

class qwtCurvePreClass
{
public:    
    QVector<double> DataX;

    QString curveTypeName;
    QColor color;   
    QVector<double> Y;
};

class qwtAllCurvePreClass
{
public:
    explicit qwtAllCurvePreClass()
    {
//       pidData.resize(PID_CURVE_SIZE);
    }

    QVector <qwtCurvePreClass> pidData;


    int pidCurveDataSize;
    QVector<double> pidDataX;
    double pidDataXtemp;

    bool pidDataProFlag;
    double pidFromX_DisplaySize;
    double pidFromGrep_x;

};

class G_dataProClass : public QObject
{
    Q_OBJECT
public:
    explicit G_dataProClass(QObject *parent = 0);

    //about From data pro    
    qwtAllCurvePreClass qwtAllCurvePre;

    bool qwtFromDataPro(class allRxDataClass *allRxData);

signals:
    void fromDataSignal(class qwtCurvePreClass *curvePreArry,const double *DataX,
                        const int dataDize,const int curveSize);

    void qwtPidFromDataSignal(qwtAllCurvePreClass *allCurve);

public slots:

};

extern G_dataProClass G_dataPro;

//qRegisterMetaType<class curvePreStruct>("class curvePreStruct");
Q_DECLARE_METATYPE(class qwtCurvePreClass);
Q_DECLARE_METATYPE(class qwtAllCurvePreClass);
#endif // G_DATAPRO_H
