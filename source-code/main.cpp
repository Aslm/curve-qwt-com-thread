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

#include <QtGlobal>
#if QT_VERSION >= 0x050000
#include <QApplication>
#else
#include <QtGui/QApplication>
#endif
#include "mainwindow.h"
#include <QTextCodec>
#include <QDebug>
#include <QDir>

#include "g_debug.h"
#include "myserial/workerserial.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Lets register our custom handler, before we start
    qInstallMsgHandler(customMessageHandler);
    qDebug() << "***************************" ;

#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
#endif

#if 1
    qDebug()<<"main thread:"<<QThread::currentThreadId();
#endif

    MainWindow w;

    //配置 当主窗口关闭时触发 quit函数
    a.connect(&a, SIGNAL(lastWindowClosed()), &w, SLOT(quit()));

    w.show();

    return a.exec();
}
