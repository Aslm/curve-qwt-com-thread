#include "pidform.h"
#include "ui_pidform.h"

#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_point_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_text.h>
#include <qwt_symbol.h>
#include <qwt_math.h>
#include <QSettings>
#include <QColorDialog>

#include "g_datapro.h"

pidForm::pidForm(QWidget *parent) :
    QWidget(parent),
    formShowFlag(false),
    ui(new Ui::pidForm)
{
    ui->setupUi(this);
    setWindowTitle(tr("Curve View"));

    QStringList curveList;
    QColor colorArry[PID_CURVE_DEFAULT_SIZE]={
        QColor( 250 , 0 , 0 ) ,
        QColor( 0 , 85 , 0 ) ,
        QColor( 170 , 85 , 0 ) ,
        QColor( 0 , 170 , 0 ) ,
        QColor( 170 , 170 , 0 ) ,
        QColor( 170 , 255 , 0 )
    };
    pidCurveName.resize(PID_CURVE_DEFAULT_SIZE);
    pidCurveColor.resize(PID_CURVE_DEFAULT_SIZE);

    //读取配置文件
    QSettings *configIniRead = new QSettings("./cfg/curveCfg.ini", QSettings::IniFormat);

    double SampleGrp = configIniRead->value("/curve/SampleGrp").toDouble();
    double SampleXsize = configIniRead->value("/curve/SampleXsize").toDouble();
    double yAxisMax = configIniRead->value("/curve/yAxisMax").toDouble();
    double yAxisMin = configIniRead->value("/curve/yAxisMin").toDouble();

    pidCurveName[0] = configIniRead->value("/curve/curve1/name").toString();
    pidCurveName[1] = configIniRead->value("/curve/curve2/name").toString();
    pidCurveName[2] = configIniRead->value("/curve/curve3/name").toString();
    pidCurveName[3] = configIniRead->value("/curve/curve4/name").toString();
    pidCurveName[4] = configIniRead->value("/curve/curve5/name").toString();
    pidCurveName[5] = configIniRead->value("/curve/curve6/name").toString();

    QVector<QString> color;
    color.push_back(configIniRead->value("/curve/curve1/color").toString());
    color.push_back(configIniRead->value("/curve/curve2/color").toString());
    color.push_back(configIniRead->value("/curve/curve3/color").toString());
    color.push_back(configIniRead->value("/curve/curve4/color").toString());
    color.push_back(configIniRead->value("/curve/curve5/color").toString());
    color.push_back(configIniRead->value("/curve/curve6/color").toString());

    bool ok;
    int num;
    for(int i = 0; i < color.size(); ++i)
    {
        num = color.at(i).mid(1,6).toInt(&ok,16);
        colorArry[i].setRed(num >> 16 & 0xFF);
        colorArry[i].setGreen(num >> 8 & 0xFF);
        colorArry[i].setBlue(num >> 0 & 0xFF);
        pidCurveColor[i] = colorArry[i];
    }

//    qDebug() << colorArry[0].red() << colorArry[0].green() << colorArry[0].blue();

    //配置窗口控件值
    ui->sampleXgrpDoubleSpinBox->setValue(SampleGrp);
    ui->sampleXsizeSpinBox->setValue(SampleXsize);
    ui->pidYmaxDoubleSpinBox->setValue(yAxisMax);
    ui->pidYminDoubleSpinBox->setValue(yAxisMin);

    //配置参数值 与 上面控件值想对应
    G_dataPro.qwtAllCurvePre.pidFromGrep_x = SampleGrp;
    G_dataPro.qwtAllCurvePre.pidCurveDataSize = SampleXsize;
    pidXmin = 0;
    pidXmax = G_dataPro.qwtAllCurvePre.pidCurveDataSize * G_dataPro.qwtAllCurvePre.pidFromGrep_x;
    pidYmax = yAxisMax;
    pidYmin = yAxisMin;


    curveList.clear();
    for(int i = 0; i < PID_CURVE_DEFAULT_SIZE; ++ i)
    {
        curveList.append(pidCurveName.at(i));

        ui->DisplyQwtPlot_1->addPlotCurve(pidCurveName.at(i),
                                          colorArry[i]);
    }

    ui->curveComboBox->addItems(curveList);

    ui->curveNameLineEdit->setText(pidCurveName.at(0));
    QPalette palette;
    palette.setColor(QPalette::Text,pidCurveColor.at(0));
    ui->curveNameLineEdit->setPalette(palette);

    G_dataPro.qwtAllCurvePre.pidDataProFlag = false;
    ui->pidStartPushButton->setText(tr("Stop"));

    //配置初始曲线参数
    ui->DisplyQwtPlot_1-> setAxisPlotScale(0,pidXmax,0,pidYmin,pidYmax,0);
}

pidForm::~pidForm()
{
    delete ui;
}


void pidForm::receiveQwtFromData(qwtAllCurvePreClass *allCurvePre)
{
    if(formShowFlag == true)
    {
#if 0
        qDebug() << "FormShowFlag == true";
#endif

        ui->DisplyQwtPlot_1->rePlotCurveDynMore(allCurvePre);
    }
}

void pidForm::closeEvent(QCloseEvent *event)
{
    event = event;

    formShowFlag = false;
    G_dataPro.qwtAllCurvePre.pidDataProFlag = false;
    ui->pidStartPushButton->setText(tr("Stop"));
    qDebug()<< P_LIGHT_RED << "pidForm was close" << P_NONE;
}

void pidForm::on_pidStartPushButton_clicked()
{
    if(ui->pidStartPushButton->text() == tr("Stop"))
    {
        G_dataPro.qwtAllCurvePre.pidDataProFlag = true;
        ui->pidStartPushButton->setText(tr("Start"));
    }
    else
    {
        G_dataPro.qwtAllCurvePre.pidDataProFlag = false;
        ui->pidStartPushButton->setText(tr("Stop"));
    }
}

void pidForm::on_curveComboBox_currentIndexChanged(int index)
{
    ui->curveNameLineEdit->setText(pidCurveName.at(index));
    QPalette palette;
    palette.setColor(QPalette::Text,pidCurveColor.at(index));
    ui->curveNameLineEdit->setPalette(palette);
}



void pidForm::on_setCurveNamePushButton_clicked()
{
    int index = ui->curveComboBox->currentIndex();

    QString name = ui->curveNameLineEdit->text();
    ui->curveComboBox->setItemText(index,name);
    pidCurveName[index] = name;

    QPalette palette = ui->curveNameLineEdit->palette();
    QColor color = palette.color(QPalette::Text);
    pidCurveColor[index] = color;
#if 1
    qDebug() << ui->curveComboBox->itemText(index) << "color " << color;
#endif
}


void pidForm::on_colorPushButton_clicked()
{
    QColor color = QColorDialog::getColor(QColor::fromRgb(255,255,0));
#if 1
    qDebug() << "QColor(" << color.red() << "," << color.green()<< ","  << color.blue() << ")";
#endif
    QPalette palette;
    palette.setColor(QPalette::Text,color);
    ui->curveNameLineEdit->setPalette(palette);
}


void pidForm::on_resetPushButton_clicked()
{
    //删除之前的曲线连接
    ui->DisplyQwtPlot_1->restPlotCurve();
#if 1
    qDebug() << "curveQVector restPlotCurve()" << ui->DisplyQwtPlot_1->curveQVector.size();
#endif

    //重置检查
    G_dataPro.qwtAllCurvePre.pidDataProFlag = false;

    if(pidCurveName.size() != pidCurveColor.size() || pidCurveName.size() != ui->curveComboBox->count())
    {
        qDebug()<< P_RED << "pidCurveName.size() != pidCurveCplor.size || "
                << "pidCurveName.size() != ui->curveComboBox->count()" << P_NONE;
        return;
    }
    //重置
    for(int i = 0; i < ui->curveComboBox->count();i++)
    {
        ui->DisplyQwtPlot_1->addPlotCurve(pidCurveName.at(i),pidCurveColor.at(i));
    }
    //曲线配置
    G_dataPro.qwtAllCurvePre.pidDataXtemp = 0;
    G_dataPro.qwtAllCurvePre.pidDataX.clear();
    ui->DisplyQwtPlot_1-> setAxisPlotScale(pidXmin,pidXmax,0,pidYmin,pidYmax,0);

    G_dataPro.qwtAllCurvePre.pidDataProFlag = true;

#if 1
    qDebug() << "curveQVector addPlotCurve()" << ui->DisplyQwtPlot_1->curveQVector.size();
#endif
}

void pidForm::on_sampleXgrpDoubleSpinBox_valueChanged(double arg1)
{
    G_dataPro.qwtAllCurvePre.pidFromGrep_x = arg1;

#if 0
    qDebug() << "sample x grp " << G_dataPro.qwtAllCurvePre.pidFromGrep_x;
#endif
}

void pidForm::on_sampleXsizeSpinBox_valueChanged(int arg1)
{
    G_dataPro.qwtAllCurvePre.pidFromX_DisplaySize = arg1;
    pidXmax = arg1;

    ui->DisplyQwtPlot_1->setAxisPlotScale(pidXmin,pidXmax,0,pidYmin,pidYmax,0);

#if 0
    qDebug() << "sample x size" << pidXmax;
#endif

}

void pidForm::on_pidYminDoubleSpinBox_valueChanged(double arg1)
{
    pidYmin = arg1;

    ui->DisplyQwtPlot_1-> setAxisPlotScale(pidXmin,pidXmax,0,pidYmin,pidYmax,0);
}

void pidForm::on_pidYmaxDoubleSpinBox_valueChanged(double arg1)
{
    pidYmax = arg1;

    ui->DisplyQwtPlot_1-> setAxisPlotScale(pidXmin,pidXmax,0,pidYmin,pidYmax,0);
}

void pidForm::on_saveCfgPushButton_clicked()
{
    //写入配置文件
    QSettings *configIniWrite  = new QSettings("./cfg/curveCfg.ini", QSettings::IniFormat);
    configIniWrite->setValue("/curve/SampleGrp", ui->sampleXgrpDoubleSpinBox->value());
    configIniWrite->setValue("/curve/SampleXsize", ui->sampleXsizeSpinBox->value());
    configIniWrite->setValue("/curve/yAxisMax", ui->pidYmaxDoubleSpinBox->value());
    configIniWrite->setValue("/curve/yAxisMin", ui->pidYminDoubleSpinBox->value());

    configIniWrite->setValue("/curve/curve1/name", pidCurveName.at(0));
    configIniWrite->setValue("/curve/curve2/name", pidCurveName.at(1));
    configIniWrite->setValue("/curve/curve3/name", pidCurveName.at(2));
    configIniWrite->setValue("/curve/curve4/name", pidCurveName.at(3));
    configIniWrite->setValue("/curve/curve5/name", pidCurveName.at(4));
    configIniWrite->setValue("/curve/curve6/name", pidCurveName.at(5));

    configIniWrite->setValue("/curve/curve1/color", pidCurveColor.at(0));
    configIniWrite->setValue("/curve/curve2/color", pidCurveColor.at(1));
    configIniWrite->setValue("/curve/curve3/color", pidCurveColor.at(2));
    configIniWrite->setValue("/curve/curve4/color", pidCurveColor.at(3));
    configIniWrite->setValue("/curve/curve5/color", pidCurveColor.at(4));
    configIniWrite->setValue("/curve/curve6/color", pidCurveColor.at(5));
    //写入完成后删除指针
    delete configIniWrite;
}
