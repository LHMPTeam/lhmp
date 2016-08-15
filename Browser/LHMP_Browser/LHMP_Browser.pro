#-------------------------------------------------
#
# Project created by QtCreator 2015-02-27T17:08:37
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LHMP_Browser
TEMPLATE = app


SOURCES += main.cpp\
        launcher.cpp \
    clhmpquery.cpp \
    udpwrapper.cpp \
    settings.cpp

HEADERS  += launcher.h \
    clhmpquery.h \
    udpwrapper.h \
    settings.h

FORMS    += launcher.ui \
    settings.ui

RESOURCES += \
    resources.qrc

LIBS += -lwsock32

DISTFILES += \
    launcher.rc

RC_FILE = launcher.rc

win32:LIBS += -luser32
QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
