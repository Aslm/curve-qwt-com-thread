#ifndef PIDFORM_H
#define PIDFORM_H

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

#include <QWidget>
#include "g_datapro.h"

#define PID_CURVE_DEFAULT_SIZE   (6)

namespace Ui {
class pidForm;
}

class pidForm : public QWidget
{
    Q_OBJECT

public:
    explicit pidForm(QWidget *parent = 0);
    ~pidForm();

    //窗口线程显示标志位，用于判断是否要处理相关窗口数据
    bool formShowFlag;

    QVector<QString> pidCurveName;
    QVector<QColor> pidCurveColor;
    double pidXmin;
    double pidXmax;
    double pidYmin;
    double pidYmax;

private:
    Ui::pidForm *ui;

private slots:
    //qwt 窗口接受数据并绘制函数
    void receiveQwtFromData(class qwtAllCurvePreClass *allCurvePre);

    void on_pidStartPushButton_clicked();

    void on_curveComboBox_currentIndexChanged(int index);

    void on_setCurveNamePushButton_clicked();

    void on_colorPushButton_clicked();

    void on_resetPushButton_clicked();

    void on_sampleXgrpDoubleSpinBox_valueChanged(double arg1);

    void on_sampleXsizeSpinBox_valueChanged(int arg1);

    void on_pidYminDoubleSpinBox_valueChanged(double arg1);

    void on_pidYmaxDoubleSpinBox_valueChanged(double arg1);

    void on_saveCfgPushButton_clicked();

protected:
    //窗口关闭函数重定义
    void closeEvent(QCloseEvent *event);
};

#endif // PIDFORM_H
