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

#ifndef SERVERTABLEWIDGET_H
#define SERVERTABLEWIDGET_H

#include <QDebug>
#include <QFutureWatcher>
#include <QHeaderView>
#include <QPainter>
#include <QTableView>

#include "../nwnmasterserver.h"
#include "../server.h"
#include "../models/servertablemodel.h"
#include "../models/ServerTableProxyModel.h"
#include "serversettings.h"

class QTimer;
class Options;
class TextBoxDialog;
class ServerSettingsDialog;

class HeaderView : public QHeaderView {
    Q_OBJECT

public:
    HeaderView(Qt::Orientation orientation, QWidget * parent = 0)
        : QHeaderView(orientation, parent), p(":/qss_icons/rc/padlock_closed.png") {
    }

    int sectionSizeHint ( int /*logicalIndex*/ ) { return p.width(); }

protected:
    void paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const {
        auto x = p.scaled(rect.width()-8, rect.height()-8, Qt::KeepAspectRatio);
        if (logicalIndex == 0)
            painter->drawPixmap(
                        (rect.width() - x.width()) / 2,
                        rect.y()-3 + (rect.height() - x.height()),
                        x);

        QHeaderView::paintSection(painter, rect, logicalIndex);
    }

private:
    QPixmap p;
};


class ServerTableWidget : public QTableView {
    Q_OBJECT

    typedef QFutureWatcher<std::vector<Server>> ServerFutureWatcher;

    int current_room_;
    int requested_room_;
    Options *options_;
    ServerFutureWatcher watcher_;
    QString current_cat_;
    QTimer *timer_;
    QTimer *ping_timer_;
    QTimer *offline_timer_;
    ServerSettingsDialog *server_settings_dlg_;
    ServerTableModel *model_;
    ServerTableProxyModel *proxy_model_;
    bool error_loading_;

    bool canUpdate();
    void setPackets();

public:
    explicit ServerTableWidget(Options *options, QWidget *parent = 0);

    QVariant getSelectedServerInfo(ServerTableModel::UserRoles role);
    void getServerList(int room, bool force = false);
    void loadServers(int room, bool force = false);
    void setServerAddressFilter(const QStringList &ips, const QString &cat);
    void updateBlacklist();

public slots:
    void addServer(const QString &addr);
    void finished();
    void onSettingsChanged();

signals:
    void dm();
    void play();
    void requestAddTo();
    void requestRemoveFrom();
    void serverInfoRequest(ServerInfoTypes type);
    void update();

private slots:
    void customMenuRequested(QPoint pos);
    void onBlacklist();
    void onModelDataChanged(QModelIndex, QModelIndex);
    void onRemoveFromCat();
    void requestChangeServerSettings();
    void requestUpdates();
    void sweepOfflineServers();

};

#endif // SERVERTABLEWIDGET_H
