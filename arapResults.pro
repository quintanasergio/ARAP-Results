#-------------------------------------------------
#
# Project created by QtCreator 2017-04-16T13:32:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = arapResults
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    barra.cpp \
    dialogmodel.cpp \
    animation.cpp \
    ant.cpp \
    node.cpp \
    dialogtable.cpp

HEADERS  += mainwindow.h \
    barra.h \
    dialogmodel.h \
    animation.h \
    ant.h \
    node.h \
    dialogtable.h

FORMS    += mainwindow.ui \
    dialogmodel.ui \
    animation.ui \
    dialogtable.ui

INCLUDEPATH += /usr/include/qwt-qt4

LIBS += -l qwt-qt4
