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
    providers/youget.cpp \
    videoaddress.cpp \
    players/mpv.cpp \
    playerloader.cpp \
    3rd-party/waitingspinnerwidget.cpp

HEADERS  += mainwindow.h \
    rpcserver.h \
    providers/youget.h \
    videoaddress.h \
    providers/videoprovider.h \
    players/player.h \
    players/mpv.h \
    playerloader.h \
    3rd-party/waitingspinnerwidget.h

FORMS    += mainwindow.ui \
    playerloader.ui
