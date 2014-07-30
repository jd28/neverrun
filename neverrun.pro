#-------------------------------------------------
#
# Project created by QtCreator 2014-07-09T02:25:58
#
#-------------------------------------------------

TEMPLATE = subdirs

win32 {
    SUBDIRS = 3rdparty \
        app \

    app.depends = 3rdparty app-static
}

unix {
    SUBDIRS = app

    app.depends = app-static
}

SUBDIRS += \
    app-static

