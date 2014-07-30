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

#include "mainwindow.h"
#include "widgets/setdmpassworddialog.h"
#include "options.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QFile f(":/qdarkstyle/style.qss");
    f.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(f.readAll());
    a.setStyleSheet(styleSheet);

    MainWindow w;
    w.readSettings();

    if(!w.AddServers()){
        qDebug() << "Failed to load servers...";
    }
    w.show();

    return a.exec();
}
