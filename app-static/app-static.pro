#
# Application Static Libary Project for neverrun
#

QT += gui

TARGET = app-static
TEMPLATE = lib
CONFIG += staticlib

!include("hoedown/hoedown.pri") {
        error("Unable to include hoedown.")
}

SOURCES += \
    converter/hoedownmarkdownconverter.cpp

HEADERS += \
    converter/markdownconverter.h \
    converter/markdowndocument.h \
    converter/hoedownmarkdownconverter.h

##################################################
# Dependencies
##################################################

INCLUDEPATH += $$PWD/../3rdparty/hoedown/
