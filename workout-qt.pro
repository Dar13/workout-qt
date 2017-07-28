#-------------------------------------------------
#
# Project created by QtCreator 2017-04-16T18:47:46
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = workout-qt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include/

SOURCES += source/main.cpp\
           source/mainwindow.cpp \
           source/countdownmodel.cpp \
           source/dbmanager.cpp \
           source/workoutlogdialog.cpp \
           source/exerciseselectdialog.cpp \
           source/exerciseinfodialog.cpp \
           source/setinfodialog.cpp \
           source/dbtypes.cpp

HEADERS += include/mainwindow.h \
           include/countdownmodel.h \
           include/dbmanager.h \
           include/dbtypes.h \
           include/workoutlogdialog.h \
           include/exerciseselectdialog.h \
           include/exerciseinfodialog.h \
           include/common.h \
           include/setinfodialog.h

FORMS   += ui/mainwindow.ui \
           ui/workoutlogdialog.ui \
           ui/exerciseselectdialog.ui \
           ui/exerciseinfodialog.ui \
           ui/setinfodialog.ui
