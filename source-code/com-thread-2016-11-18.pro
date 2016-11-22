#-------------------------------------------------
#
# Project created by QtCreator 2016-10-14T01:28:51
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = com-thread-2016-11-14
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qextserial/qextserialport.cpp \
    pidform.cpp \
    myserial/workerserial.cpp \
    g_datapro.cpp \
    myqwtplot/myqwtplot.cpp \
    g_debug.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    qextserial/qextserialport.h \
    qextserial/qextserialport_global.h \
    pidform.h \
    myserial/workerserial.h \
    qextserial/qextserialport_p.h \
    g_datapro.h \
    g_define.h \
    myqwtplot/myqwtplot.h \
    g_debug.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    pidform.ui \
    aboutdialog.ui

win32 {
     SOURCES += qextserial/qextserialport_win.cpp
}

unix {
     SOURCES += qextserial/qextserialport_unix.cpp
}

#qwt setting
#qwt控件安装位置，注意与qwtconfig.pri配置文件配合修改
QWT_ROOT = C:/1-Need-Soft/Qt/Qwtlib-6.1.2

include ( $${QWT_ROOT}/features/qwt.prf )
#end qwt setting

RESOURCES += \
    icon_resource.qrc

RC_FILE = app.rc




