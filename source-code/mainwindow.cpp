#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSettings>

#include "g_datapro.h"

#define  DIAPLAY_SIZE    (4000)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Assistant"));

    //读取配置文件
    QSettings *configIniRead = new QSettings("./cfg/seriaCfg.ini", QSettings::IniFormat);
    QString seriaName = configIniRead->value("/seria/Name").toString();
    QString seriaBaudRate = configIniRead->value("/seria/BaudRate").toString();
    QString seriaDataBits = configIniRead->value("/seria/DataBits").toString();
    QString seriaStopBits = configIniRead->value("/seria/StopBits").toString();
    QString seriaRepeatTime = configIniRead->value("/seria/RepeatTime").toString();

    delete configIniRead;

#if 1
    qDebug() << seriaName << seriaBaudRate << seriaDataBits <<seriaStopBits << seriaRepeatTime;
#endif

    ui->portNameLineEdit->setText(seriaName);

    int i;
    i = ui->baudRateComboBox->findText(seriaBaudRate);
    if(i != -1)
        ui->baudRateComboBox->setCurrentIndex(i);

    i = ui->dataBitsComboBox->findText(seriaDataBits);
    if(i != -1)
        ui->dataBitsComboBox->setCurrentIndex(i);

    i = ui->stopBitsComboBox->findText(seriaStopBits);
    if(i != -1)
        ui->stopBitsComboBox->setCurrentIndex(i);

    ui->repeatDelaySpinBox->setValue(seriaRepeatTime.toDouble());
    repeatSentDelay = seriaRepeatTime.toDouble();

    //界面初始化
    ui->closeSerialPushButton->setEnabled(false);
    ui->hexSentCheckBox->setEnabled(false);
    ui->repeateSentCheckBox->setEnabled(false);
    ui->repeatDelaySpinBox->setEnabled(false);
    ui->sentMsgPushButton->setEnabled(false);
    ui->sendMsgTextEdit->setEnabled(false);

    //配置 当主窗口关闭是，子窗口也关闭
    this->setAttribute(Qt::WA_QuitOnClose,true);
    myPidForm.setAttribute(Qt::WA_QuitOnClose,false);

    //工作队列槽设置
    connect(&serialWorkerOne, SIGNAL(wDisplayTxDataSingle(QByteArray)),
            this, SLOT(displayRxData(QByteArray)));
    connect(&serialWorkerOne, SIGNAL(wSentSerialState(bool)),
            this, SLOT(getSerialState(bool)));

    //工作队列移至进程
    serialWorkerOne.moveToThread(&serialThreadOne);
    serialThreadOne.start();

    //数据处理 信号 与 相对应的窗口重绘 槽 连接
    connect(&G_dataPro,SIGNAL(qwtPidFromDataSignal(qwtAllCurvePreClass*)),
            &myPidForm, SLOT(receiveQwtFromData(qwtAllCurvePreClass*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openSerialPushButton_clicked()
{

    //获取串口名
    QString portName = ui->portNameLineEdit->text();
    if(portName.isEmpty())
    {
        QMessageBox::information(this, tr("COM Name is empty! "), tr("please edit com ")
                                 + portName, QMessageBox::NoButton);
        return;
    }
    serialWorkerOne.serialCfgPreOne.name = portName;


    //连续发送初始化
    repeatTimer = new QTimer(this);
    connect(repeatTimer, SIGNAL(timeout()), this, SLOT(sentTextTxDate()));


    //设置波特率
    serialWorkerOne.serialCfgPreOne.baudRate =
            (BaudRateType)ui->baudRateComboBox->currentText().toInt();

    //设置数据位
    serialWorkerOne.serialCfgPreOne.dataBits =
            (DataBitsType)ui->dataBitsComboBox->currentText().toInt();

    //设置校验
    switch(ui->parityComboBox->currentIndex())
    {
    case 0:
        serialWorkerOne.serialCfgPreOne.parity = PAR_NONE;
        break;
    case 1:
        serialWorkerOne.serialCfgPreOne.parity = PAR_ODD;
        break;
    case 2:
        serialWorkerOne.serialCfgPreOne.parity = PAR_EVEN;
        break;
    default:
        serialWorkerOne.serialCfgPreOne.parity = PAR_NONE;
#if 1
        qDebug() << P_GREEN << serialWorkerOne.serialCfgPreOne.name
                 <<" set to default : PAR_NONE" << P_NONE;
#endif
        break;
    }

    //设置停止位
    switch(ui->stopBitsComboBox->currentIndex())
    {
    case 0:
        serialWorkerOne.serialCfgPreOne.stopBits = STOP_1;
        break;
    case 1:
#ifdef Q_OS_WIN
        serialWorkerOne.serialCfgPreOne.stopBits = STOP_1_5;
#endif
        break;
    case 2:
        serialWorkerOne.serialCfgPreOne.stopBits = STOP_2;
        break;
    default:
        serialWorkerOne.serialCfgPreOne.stopBits = STOP_1;
#if 1
        qDebug() << P_GREEN << serialWorkerOne.serialCfgPreOne.name
                 <<" set to default : STOP_1" << P_NONE;
#endif
        break;
    }

    //设置数据流控制
    serialWorkerOne.serialCfgPreOne.flow = FLOW_OFF;
    //设置延时
    serialWorkerOne.serialCfgPreOne.timeout = 20;

    serialWorkerOne.sentOpenSerialSingleFun();

    ui->statusBar->showMessage(tr("serial cfg "));
}

void MainWindow::displayRxData(QByteArray temp)
{
    QString buf;

    if(!temp.isEmpty()){
        ui->textBrowser->setTextColor(Qt::black);
        if(!(ui->hexDisCheckBox->isChecked())){
            buf = temp;
        }
        else {
            for(int i = 0; i < temp.count(); i++){
                QString s;
                s.sprintf("0x%02x ", (unsigned char)temp.at(i));
                buf += s;
            }
        }
        //该函数会影响GUI的流畅性
        //ui->textBrowser->setText(ui->textBrowser->document()->toPlainText() + buf);
        ui->textBrowser->append(buf);
        if(ui->textBrowser->document()->toPlainText().size() > DIAPLAY_SIZE)
        {
            ui->textBrowser->setText("");
        }
        QTextCursor cursor = ui->textBrowser->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->textBrowser->setTextCursor(cursor);

        ui->statusBar->showMessage(tr("ready %1 bye data ").arg(temp.size()));
#if 0
        qDebug() << "textBrowser row count" << ui->textBrowser->document()->toPlainText().size();
#endif
    }
}

void MainWindow::sentTextTxDate()
{
    QByteArray buf;

    if(ui->hexSentCheckBox->isChecked())
    {
        QString str;
        bool ok;
        char data;
        QStringList list;
        str = ui->sendMsgTextEdit->toPlainText();
        list = str.split(" ");

#if 0
        qDebug() << "sentTextTxDate " << list;
#endif

        for(int i = 0; i < list.count(); i++){
            if(list.at(i) == " ")
                continue;
            if(list.at(i).isEmpty())
                continue;
            data = (char)list.at(i).toInt(&ok, 16);
            if(!ok){
                QMessageBox::information(this, tr("Hint message "), tr("Input data format err! "), QMessageBox::Ok);
                if(repeatTimer != NULL)
                    repeatTimer->stop();
                ui->sentMsgPushButton->setText(tr("sentMsg "));
                //                ui->sendmsgBtn->setIcon(QIcon(":new/prefix1/src/send.png"));
                return;
            }
            buf.append(data);
        }
    }
    else
    {
#if QT_VERSION < 0x050000
        buf = ui->sendMsgTextEdit->toPlainText().toAscii();
#else
        buf = ui->sendMsgLineEdit->text().toLocal8Bit();
#endif

#if 0
        qDebug() << " " << "sendMsgTextEdit" << buf;
#endif
    }

    //发送数据
    serialWorkerOne.sentTxSingleFun(buf);

    //界面控制
    ui->statusBar->showMessage(tr("sent success "));
    ui->textBrowser->setTextColor(Qt::lightGray);
}

void MainWindow::quit()
{
#if 1
    qDebug() << P_NONE << "MainWindow " << "quit!" << P_NONE;
#endif

    serialThreadOne.exit();
    serialThreadOne.wait();
}

void MainWindow::getSerialState(bool state)
{
    if(state == true)
    {
        QMessageBox::information(this, tr("open success "),
                                 serialWorkerOne.serialCfgPreOne.name +
                                 tr(" ready ") , QMessageBox::Ok);
        //界面控制
        ui->openSerialPushButton->setEnabled(false);
        ui->closeSerialPushButton->setEnabled(true);

        ui->hexSentCheckBox->setEnabled(true);
        ui->repeateSentCheckBox->setEnabled(true);
        ui->repeatDelaySpinBox->setEnabled(true);
        ui->sentMsgPushButton->setEnabled(true);
        ui->sendMsgTextEdit->setEnabled(true);
    }
    else
    {
        QMessageBox::critical(this, tr("open fail "),
                              serialWorkerOne.serialCfgPreOne.name +
                              tr("no ready ") + tr("\nthis serial non-existent "), QMessageBox::Ok);
        return;
    }
}

void MainWindow::on_closeSerialPushButton_clicked()
{
    serialWorkerOne.sentCloseSerialSingleFun();

    if(repeatTimer->isActive())
        repeatTimer->stop();

    if(repeatTimer != NULL)
        delete repeatTimer;

    repeatTimer= NULL;

    //界面控制
    ui->openSerialPushButton->setEnabled(true);
     ui->closeSerialPushButton->setEnabled(false);

    ui->hexSentCheckBox->setEnabled(false);
    ui->repeateSentCheckBox->setEnabled(false);
    ui->repeatDelaySpinBox->setEnabled(false);
    ui->sentMsgPushButton->setEnabled(false);
    ui->sendMsgTextEdit->setEnabled(false);

    ui->sentMsgPushButton->setText(tr("sentMsg"));

    ui->statusBar->showMessage(tr("serial close "));

}

void MainWindow::on_sentMsgPushButton_clicked()
{
    //如果当前正在连续发送数据，暂停发送
    if(ui->sentMsgPushButton->text() == tr("suspend")){
        repeatTimer->stop();
        ui->sentMsgPushButton->setText(tr("sentMsg"));
        //        ui->sendmsgBtn->setIcon(QIcon(":new/prefix1/src/send.png"));
        return;
    }
    //如果发送数据为空，给出提示并返回
    if(ui->sendMsgTextEdit->toPlainText().isEmpty()){
        QMessageBox::information(this, tr("Hint message"), tr("data no ready"), QMessageBox::Ok);
        return;
    }

    //如果不是连续发送
    if(!ui->repeateSentCheckBox->isChecked()){
        ui->sentMsgPushButton->setFocus();
        //发送数据
        sentTextTxDate();
    }else{ //连续发送
        repeatTimer->start(repeatSentDelay);
        ui->sentMsgPushButton->setText(tr("suspend"));
        //        ui->sendmsgBtn->setIcon(QIcon(":new/prefix1/src/pause.png"));
    }
}

void MainWindow::on_repeateSentCheckBox_clicked()
{
    if(ui->repeateSentCheckBox->isChecked()){
        ui->repeatDelaySpinBox->setEnabled(true);
        //检查是否有数据，如有则启动定时器
        ui->statusBar->showMessage(tr("start repeate data"));
    }else{
        ui->repeatDelaySpinBox->setEnabled(false);
        //若定时器已经启动则关闭它
        ui->statusBar->showMessage(tr("stop repeate data"));
    }
}

void MainWindow::on_repeatDelaySpinBox_valueChanged(int arg1)
{
    repeatSentDelay = arg1;
}

void MainWindow::on_clearTextPushButton_clicked()
{
    ui->textBrowser->clear();
    ui->statusBar->showMessage(tr("text bos wsa empty"));
}

void MainWindow::on_saveCfgPushButton_clicked()
{
    //写入配置文件
    QSettings *configIniWrite  = new QSettings("./cfg/seriaCfg.ini", QSettings::IniFormat);
    configIniWrite->setValue("/seria/Name", ui->portNameLineEdit->text());
    configIniWrite->setValue("/seria/BaudRate", ui->baudRateComboBox->currentText());
    configIniWrite->setValue("/seria/DataBits", ui->dataBitsComboBox->currentText());
    configIniWrite->setValue("/seria/StopBits", ui->stopBitsComboBox->currentText());
    configIniWrite->setValue("/seria/RepeatTime", ui->repeatDelaySpinBox->value());
    //写入完成后删除指针
    delete configIniWrite;
}

void MainWindow::on_actionCurveView_triggered()
{
    myPidForm.show();
    myPidForm.formShowFlag = true;

    // 在主窗口中间显示
    //    int x =this->x() + (this->width() - myPidForm.width()) / 2;
    //    int y =this->y() + (this->height() - myPidForm.height()) / 2;
    //    myPidForm.move(x, y);
    ui->statusBar->showMessage(tr("qwt curve start"));
}

void MainWindow::on_actionAbout_ass_triggered()
{
    aboutdlg.show();
    // 在主窗口中间显示
    int x =this->x() + (this->width() - aboutdlg.width()) / 2;
    int y =this->y() + (this->height() - aboutdlg.height()) / 2;
    aboutdlg.move(x, y);
    ui->statusBar->showMessage(tr("关于Wincom"));
}
