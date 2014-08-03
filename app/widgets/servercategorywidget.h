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

#ifndef SERVERCATEGORYWIDGET_H
#define SERVERCATEGORYWIDGET_H

#include <QTableWidget>
#include <QWidget>

class Options;

class ServerCategoryWidget : public QTableWidget {
    Q_OBJECT

    Options *options_;

    void removeCategory(const QString& cat);

public:
    ServerCategoryWidget(Options *settings, QWidget *parent = 0);
    void SelectAll();
    void addCategory(const QString& cat);

signals:
    void LoadAllServers(int room);
    void UpdateFilter(const QStringList& ips, const QString& name);

public slots:
    void ChangeServerList();
private slots:
    void customMenuRequested(QPoint pos);
    void onRemoveCategory();
};

#endif // SERVERCATEGORYWIDGET_H
