#include "g_debug.h"
#include <QDir>

#define _TIME_ qPrintable (QDateTime::currentDateTime().toString ("hh-mm-ss"))

void customMessageHandler(QtMsgType type, const char *msg)
{
    QString qstrText;
    switch (type)
    {
    case QtDebugMsg:
        qstrText = QString("%1: Debug: %2").arg(_TIME_, msg);
        break;
    case QtWarningMsg:
        qstrText = QString("%1: Warning: %2").arg(_TIME_, msg);
        break;
    case QtCriticalMsg:
        qstrText = QString("%1: Critical: %2").arg(_TIME_, msg);
        break;
    case QtFatalMsg:
        qstrText = QString("%1: Fatal: %2").arg(_TIME_, msg);
        exit(0);
    }

    QDir *temp = new QDir;
    bool exist = temp->exists("./log");
    if(exist)
    {
        //            qWarning("cannot create ./log");
    }
    else
    {
        bool ok = temp->mkdir("./log");
        if( ok )
        {
            //                qDebug() << "./log ok";
        }
    }

    QString data = QDateTime::currentDateTime().toString ("yyyy-MM-dd");
    QString logfile = "./log/" + data + "-log.txt";

    QFile log(logfile);
    log.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&log);
    ts << qstrText << endl;
}
