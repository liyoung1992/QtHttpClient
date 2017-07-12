#-------------------------------------------------
#
# Project created by QtCreator 2014-12-10T21:00:12
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtHttpTest
TEMPLATE = app


SOURCES += main.cpp\
        MainDialog.cpp \
    HttpFun.cpp \
    EditWidget.cpp

HEADERS  += MainDialog.h \
    HttpFun.h \
    EditWidget.h \
    json/src/json.hpp

FORMS    += MainDialog.ui \
    EditWidget.ui

RESOURCES += \
    images.qrc
