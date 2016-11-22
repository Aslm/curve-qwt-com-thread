#ifndef MYQWTPLOT_H
#define MYQWTPLOT_H

#include <QObject>

#include "qwt_plot.h"
#include <qwt_plot_curve.h>
#include <g_datapro.h>

class myQwtPlotClass : public QwtPlot
{
    Q_OBJECT

public:
    myQwtPlotClass( QWidget *parent = NULL );

    QVector<QwtPlotCurve *> curveQVector;

    //增加 与 重置 函数
    QwtPlotCurve *addPlotCurve( const QString &title, const QColor &curveColor );
    void restPlotCurve();

    //重置 qwt 控件坐标最大、最小值
    void setAxisPlotScale(double x_min, double x_max ,double x_step,
                          double y_min, double y_max ,double y_step);

    bool setPlotCurveColor(const QString &title,const QColor &curveColor);

    //动态绘制函数
    bool rePlotCurveDynMore(class qwtAllCurvePreClass *pidCurvePre);


private:


private slots:
    //qwt 控件曲线 显示与隐藏 槽
    void showPlotCurve(const QVariant &itemInfo, bool on);
};


#endif // MYQWTPLOT_H
