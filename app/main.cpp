/*
 * Copyright 2014 Joshua Dean <joshua.m.dean@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QDebug>
#include <QFile>

#include "fvupdater.h"
#include "mainwindow.h"
#include "options.h"
#include "widgets/setdmpassworddialog.h"

QFile log_file;

void log(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    QTextStream stream(&log_file);
    switch (type) {
    case QtDebugMsg:
        stream << msg << "\n";
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}

int main(int argc, char *argv[]) {
    log_file.setFileName("neverrun.log");
    log_file.open(QFile::WriteOnly);
    qInstallMessageHandler(log);

    QApplication a(argc, argv);

    // Prerequisite for the Fervor updater
    QApplication::setOrganizationName("neverrun");
    QApplication::setOrganizationDomain("none");
    QApplication::setApplicationName("neverrun");
    QApplication::setApplicationVersion("0.5");

    QFile f(":/qdarkstyle/style.qss");
    f.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(f.readAll());
    a.setStyleSheet(styleSheet);

    MainWindow w;
    w.readSettings();

#ifndef QT_DEBUG
    auto updater = new FvUpdater();
    w.setUpdater(updater);

    // Set feed URL before doing anything else
    updater->SetFeedURL("https://raw.githubusercontent.com/jd28/neverrun/master/appcast_neverrun.xml");
    // Finish Up old Updates
    updater->finishUpdate();

    // Check for updates automatically
    updater->CheckForUpdatesSilent();
#endif

    if(!w.AddServers()){
        qDebug() << "Failed to load servers...";
    }
    w.show();

    return a.exec();
}
