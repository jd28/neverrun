QT       += core gui widgets webkitwidgets gui-private

TARGET = neverrun
TEMPLATE = app

DEFINES += FV_GUI=1
CONFIG += fervor_gui

#Windows XP compatability
DEFINES += "WINVER=0x0501"
DEFINES += "_WIN32_WINNT=0x0501"

# Fervor autoupdater
# (set TARGET and VERSION of your app before including Fervor.pri)
!include("../3rdparty/fervor/Fervor.pri") {
        error("Unable to include Fervor autoupdater.")
}
unix {
  CONFIG += link_pkgconfig
}

win32:INCLUDEPATH += "$(BOOST_ROOT)/include"

SOURCES += main.cpp\
    mainwindow.cpp \
    soap/soapC.cpp \
    soap/soapWSHttpBinding_USCOREINWNMasterServerAPIProxy.cpp \
    soap/stdsoap2.cpp \
    nwnmasterserver.cpp \
    models/moduletablemodel.cpp \
    models/servertablemodel.cpp \
    models/ServerTableProxyModel.cpp \
    widgets/servercategorywidget.cpp \
    htmlpreviewgenerator.cpp \
    options.cpp \
    widgets/togglebutton.cpp \
    widgets/servertablewidget.cpp \
    widgets/setdmpassworddialog.cpp \
    widgets/usernamebutton.cpp \
    widgets/listselectiondialog.cpp \
    widgets/modulecategorywidget.cpp \
    widgets/moduletablewidget.cpp \
    models/moduletableproxymodel.cpp \
    widgets/serversettings.cpp \
    widgets/serverinfowidget.cpp \
    widgets/directconnectdialog.cpp \
    util.cpp \
    Server.cpp \
    widgets/neverrun_settings.cpp \
    SingleApplication/long-lived-lock-file.cpp \
    SingleApplication/single-application.cpp

HEADERS  += mainwindow.h \
    soap/soapH.h \
    soap/soapStub.h \
    soap/soapWSHttpBinding_USCOREINWNMasterServerAPIProxy.h \
    soap/stdsoap2.h \
    nwnmasterserver.h \
    server.h \
    servertablemodel.h \
    ServerTableProxyModel.h \
    widgets/servercategorywidget.h \
    htmlpreviewgenerator.h \
    options.h \
    widgets/togglebutton.h \
    moduletablemodel.h \
    widgets/listselectiondialog.h \
    widgets/modulecategorywidget.h \
    widgets/servertablewidget.h \
    widgets/setdmpassworddialog.h \
    widgets/usernamebutton.h \
    delegates/TableSeperatorDelegate.h \
    delegates/imagedelegate.h \
    widgets/moduletablewidget.h \
    models/moduletableproxymodel.h \
    models/moduletablemodel.h \
    models/servertablemodel.h \
    models/ServerTableProxyModel.h \
    widgets/serversettings.h \
    util.h \
    widgets/serverinfowidget.h \
    widgets/directconnectdialog.h \
    widgets/neverrun_settings.h \
    SingleApplication/long-lived-lock-file.h \
    SingleApplication/single-application.h

RESOURCES += \
    qdarkstyle/style.qrc

win32:RC_ICONS += nwn1-install-disk1.ico
win32:LIBS += -lws2_32
win32:QMAKE_LFLAGS += /INCREMENTAL:NO

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../app-static/release/ -lapp-static
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../app-static/debug/ -lapp-static
else:unix: LIBS += -L$$OUT_PWD/../app-static/ -lapp-static

INCLUDEPATH += $$PWD/../3rdparty/fervor/

INCLUDEPATH += $$PWD/../app-static
DEPENDPATH += $$PWD/../app-static

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app-static/release/libapp-static.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app-static/debug/libapp-static.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app-static/release/app-static.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app-static/debug/app-static.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../app-static/libapp-static.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../3rdparty/yaml-cpp/release/ -lyaml-cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../3rdparty/yaml-cpp/debug/ -lyaml-cpp
else:unix: LIBS += -L$$OUT_PWD/../3rdparty/yaml-cpp/ -lyaml-cpp

INCLUDEPATH += $$PWD/../3rdparty/yaml-cpp/include/
DEPENDPATH += $$PWD/../3rdparty/yaml-cpp

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/yaml-cpp/release/libyaml-cpp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/yaml-cpp/debug/libyaml-cpp.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/yaml-cpp/release/yaml-cpp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/yaml-cpp/debug/yaml-cpp.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/yaml-cpp/libyaml-cpp.a

FORMS += \
    widgets/setdmpassworddialog.ui \
    widgets/listselectiondialog.ui \
    widgets/serversettings.ui \
    widgets/serverinfowidget.ui \
    widgets/directconnectdialog.ui \
    widgets/neverrun_settings.ui
