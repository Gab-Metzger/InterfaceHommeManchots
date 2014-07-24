#-------------------------------------------------
#
# Project created by QtCreator 2014-06-18T08:44:30
#
#-------------------------------------------------

QT       += core gui
QT += sql
CONFIG += QWT

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InterfaceHommeManchot
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    lecture_fichiers.cpp \
    histodialog.cpp \
    statistique.cpp \
    analyse.cpp \
    opentdmsdialog.cpp \
    ouverturemanchot.cpp \
    database.cpp \
    resultat.cpp \
    parametres.cpp

HEADERS  += mainwindow.h \
    lecture_fichiers.h \
    histodialog.h \
    statistique.h \
    analyse.h \
    opentdmsdialog.h \
    ouverturemanchot.h \
    database.h \
    resultat.h \
    parametres.h

FORMS    += mainwindow.ui \
    histodialog.ui \
    opentdmsdialog.ui \
    ouverturemanchot.ui \
    resultat.ui \
    parametres.ui

DEPENDPATH += C:\qwt-6.1.0\include\
INCLUDEPATH += C:\qwt-6.1.0\src\
LIBS += -LC:\qwt-6.1.0\lib\qwt.dll
LIBS += -LC:\qwt-6.1.0\lib\qwtd.dll
LIBS += -L"$$_PRO_FILE_PWD_\libs" -lOpenTdms
