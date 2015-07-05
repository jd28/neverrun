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

!include("yaml-cpp/yaml-cpp.pri") {
        error("Unable to include yaml-cpp.")
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
