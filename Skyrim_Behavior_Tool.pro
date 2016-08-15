#-------------------------------------------------
#
# Project created by QtCreator 2016-02-09T01:56:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Skyrim_Behavior_Tool
TEMPLATE = app

# remove possible other optimization flags
#QMAKE_CXXFLAGS_DEBUG -= -O
#QMAKE_CXXFLAGS_DEBUG -= -O1
#QMAKE_CXXFLAGS_DEBUG -= -O2

# add the desired -O3 if not present
#QMAKE_CXXFLAGS_DEBUG *= -O3

SOURCES += \
    src/fileselectwindow.cpp \
    src/generators.cpp \
    src/hkobject.cpp \
    src/hkxfile.cpp \
    src/hkxxmlreader.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/behaviorgraphui.cpp \
    src/behaviorgraphicons.cpp \
    src/hkobjectui.cpp

HEADERS  += \
    src/fileselectwindow.h \
    src/generators.h \
    src/hkobject.h \
    src/hkxfile.h \
    src/hkxxmlreader.h \
    src/mainwindow.h \
    src/utility.h \
    src/modifiers.h \
    src/behaviorgraphui.h \
    src/behaviorgraphicons.h \
    src/hkobjectui.h \
    src/genericdatawidgets.h
