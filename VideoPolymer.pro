#-------------------------------------------------
#
# Project created by QtCreator 2016-03-20T03:29:37
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoPolymer
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    rpcserver.cpp \
    providers/youget.cpp

HEADERS  += mainwindow.h \
    rpcserver.h \
    videoprovider.h \
    providers/youget.h

FORMS    += mainwindow.ui
