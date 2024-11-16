TEMPLATE = app
TARGET = CourseGraph

QT += core gui widgets

SOURCES += ../main.cpp ../Course.cpp

HEADERS += ../Course.h
CONFIG -= windows
CONFIG += console
CONFIG += c++17

INCLUDEPATH += ../
LIBS += -LC:\Qt\6.8.0\mingw_64\lib -lQt6core -lQt6Gui -lQt6Widgets