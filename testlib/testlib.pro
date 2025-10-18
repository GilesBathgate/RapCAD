TEMPLATE = app
CONFIG += console c++11
QT += testlib core
QT -= gui

TARGET = testlib

INCLUDEPATH += ../lib/src
LIBS += -L../lib -lrapcad

SOURCES += \
    main.cpp
