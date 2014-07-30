#
# Application Static Libary Project for neverrun
#

QT += gui

TARGET = app-static
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    converter/discountmarkdownconverter.cpp \

HEADERS += \
    converter/markdownconverter.h \
    converter/markdowndocument.h \
    converter/discountmarkdownconverter.h \

#unix:!symbian {
#    maemo5 {
#        target.path = /opt/usr/lib
#    } else {
#        target.path = /usr/lib
#    }
#    INSTALLS += target
#}

##################################################
# Dependencies
##################################################

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../3rdparty/discount/release/ -ldiscount
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../3rdparty/discount/debug/ -ldiscount
else:unix: LIBS += -L$$OUT_PWD/../3rdparty/discount/ -ldiscount

INCLUDEPATH += $$PWD/../3rdparty/discount
DEPENDPATH += $$PWD/../3rdparty/discount

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/discount/release/libdiscount.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/discount/debug/libdiscount.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/discount/release/discount.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/discount/debug/discount.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/discount/libdiscount.a
