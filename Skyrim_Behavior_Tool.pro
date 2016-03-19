#-------------------------------------------------
#
# Project created by QtCreator 2016-02-09T01:56:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Skyrim_Behavior_Tool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fileselectwindow.cpp \
    hkxfile.cpp \
    hkobject.cpp \
    generators.cpp \
    hkxxmlreader.cpp

HEADERS  += mainwindow.h \
    fileselectwindow.h \
    hkxfile.h \
    hkobject.h \
    utility.h \
    generators.h \
    hkxxmlreader.h
