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

#include "myqwtplot/myqwtplot.h"

#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_point_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_zoomer.h>
#include <qwt_text.h>
#include <qwt_symbol.h>
#include <qwt_math.h>

#include "g_define.h"


myQwtPlotClass::myQwtPlotClass( QWidget *parent ):
    QwtPlot( parent )
{
    setAutoFillBackground( true );
    //    setPalette( QPalette( QColor( 165, 193, 228 ) ) );

    setTitle(tr("Curve View") );
    insertLegend( new QwtLegend(), QwtPlot::RightLegend );
    // axes
    setAxisTitle( xBottom, "x" );
    setAxisTitle( yLeft,   "y" );
    setAxisPlotScale(0,5,0,-5,5,0);

    //  ...a horizontal line at y = 0...
    QwtPlotMarker *mY = new QwtPlotMarker();
    mY->setLabel( QString::fromLatin1( "y = 0" ) );
    mY->setLabelAlignment( Qt::AlignRight | Qt::AlignTop );
    mY->setLineStyle( QwtPlotMarker::HLine );
    mY->setYValue( 0.0 );
    mY->attach( this );

    //  ...a vertical line at x = 2 * pi
    //    QwtPlotMarker *mX = new QwtPlotMarker();
    //    mX->setLabel( QString::fromLatin1( "x = 2 pi" ) );
    //    mX->setLabelAlignment( Qt::AlignLeft | Qt::AlignBottom );
    //    mX->setLabelOrientation( Qt::Vertical );
    //    mX->setLineStyle( QwtPlotMarker::VLine );
    //    mX->setLinePen( Qt::black, 0, Qt::DashDotLine );
    //    mX->setXValue( 2.0 * M_PI );
    //    mX->attach( this );

    // canvas
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setLineWidth( 1 );
    canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    canvas->setPalette( canvasPalette );

    setCanvas( canvas );

    // panning with the left mouse button
    ( void ) new QwtPlotPanner( canvas );

    // zoom in/out with the wheel
    ( void ) new QwtPlotMagnifier( canvas );

    // zoom in/out with the wheel left button
    //    ( void ) new QwtPlotZoomer( canvas );

    // when the QwtLegend was checked than display or hide the curve
    QwtLegend *legend = new QwtLegend;
    legend->setDefaultItemMode( QwtLegendData::Checkable );
    insertLegend( legend, QwtPlot::RightLegend );

    connect(legend, SIGNAL(checked(QVariant,bool,int)),SLOT(showPlotCurve(QVariant,bool)));

    // TEST:Create sin curve
    //    addPlotCurve("test sin",Qt::blue);
    //    addPlotCurve("test cos",Qt::red);
    //    QVector<double> xs;
    //    QVector<double> ys;
    //    for (double x = 0; x < 2.0 * M_PI; x+=(M_PI / 10.0))
    //    {
    //        xs.append(x);
    //        ys.append(qSin(x));
    //    }
    //    QVector<double> xs_1;
    //    QVector<double> ys_1;
    //    for (double x_1 = 0; x_1 < 2.0 * M_PI; x_1+=(M_PI / 10.0))
    //    {
    //        xs_1.append(x_1);
    //        ys_1.append(qCos(x_1));
    //    }

    //    rePopulate("test sin",xs,ys);
    //    rePopulate("test cos",xs_1,ys_1);

}

void myQwtPlotClass::setAxisPlotScale(double x_min, double x_max, double x_step, double y_min, double y_max, double y_step)
{
    setAxisScale( xBottom, x_min, x_max, x_step );
    setAxisScale( yLeft,   y_min, y_max, y_step );
}

QwtPlotCurve *myQwtPlotClass::addPlotCurve(const QString &title, const QColor &curveColor)
{
    // Insert new curves
    QwtPlotCurve *curve = new QwtPlotCurve( title );
    curve->setRenderHint( QwtPlotItem::RenderAntialiased );
    curve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    curve->setPen( curveColor );
    curve->attach( this );

    curveQVector.push_back(curve);

    return curve;
}

void myQwtPlotClass::restPlotCurve()
{
    for(int i = 0; i < curveQVector.size();i++)
    {
        curveQVector.at(i)->detach();
    }
    curveQVector.clear();
}

bool myQwtPlotClass::setPlotCurveColor(const QString &title, const QColor &curveColor)
{
    int i = 0;

    for (i = 0; i < curveQVector.size(); ++i)
    {
        if(curveQVector.at(i)->title() == title)
            break;
    }
    if(i >= curveQVector.size())
    {
        qDebug() << P_RED <<"Err : rePopulate " << title << "false" << P_NONE;
        return false;
    }

    curveQVector.at(i)->setPen( curveColor );
    return true;
}


bool myQwtPlotClass::rePlotCurveDynMore(qwtAllCurvePreClass *pidCurvePre)
{
    if(pidCurvePre == NULL)
    {
        qDebug() << P_RED  << "warning : rePlotPidCurveDynMore pidCurvePre = NULL" << P_NONE;
        return false;
    }

    if(pidCurvePre->pidCurveDataSize > curveQVector.size())
    {
        qDebug() << P_RED  << "warning : rePlotPidCurveDynMore pidCurvePre->pidCurveSize too much"
                 << P_NONE;
        return false;
    }

    //qwt 控件 x轴 最大显示数目
    double rang = pidCurvePre->pidFromGrep_x * pidCurvePre->pidFromX_DisplaySize;
    if(pidCurvePre->pidDataX.back() >= rang)
    {
        double xAxisMax = pidCurvePre->pidDataX.back();
        double xAxisMin = pidCurvePre->pidDataX.back() - rang;
        setAxisScale( xBottom,xAxisMin,xAxisMax,0 );
    }

    //绘制曲线
    for(int i = 0;i < pidCurvePre->pidData.size();++ i)
    {
        QwtPointArrayData * const data = new QwtPointArrayData(pidCurvePre->pidDataX,
                                                               pidCurvePre->pidData.at(i).Y);
        curveQVector.at(i)->setSamples( data );
    }

    //    curveQVector.at(i)->setSamples(DataX,&curvePre[i].DataY[0],dataDize);
    //    QwtPointArrayData * const data = new QwtPointArrayData(x_AxisQVector, y_AxisQVector);
    //    curveQVector.at(i)->setSamples( data );

    this->replot();
    return true;
}

void myQwtPlotClass::showPlotCurve(const QVariant &itemInfo, bool on)
{
    QwtPlotItem *plotItem = infoToItem( itemInfo );
    if ( plotItem )
        plotItem->setVisible(on);
    this->replot();
}

