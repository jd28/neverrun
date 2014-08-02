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

    int current_room_;
    int requested_room_;
    Options *options_;
    QAction *act_add_to_;
    QAction *act_remove_from_;
    QAction *act_settings_;
    QFutureWatcher<std::vector<Server>> watcher_;
    QTimer *timer_;
    ServerSettingsDialog *server_settings_dlg_;
    ServerTableModel *model_;
    ServerTableProxyModel *proxy_model_;

    void SetupDialogs();
    void UpdatePlayerCounts();

public:
    explicit ServerTableWidget(Options *options, QWidget *parent = 0);

    void GetServerList(int room, bool force = false);
    const ServerTableProxyModel *proxyModel() { return proxy_model_; }
    void LoadServers(int room, bool force = false);
    const ServerTableModel *getServerTableModel() const;
    void SetServerAddressFilter(const QStringList &ips);

signals:
    void PasswordChanged(QString address, QString pass, bool is_dm);
    void requestAddTo();
    void requestRemoveFrom();
    void RunNWN(QString address, bool dm);
    void serverSettingsChanged();

public slots:
    void addServer(const QString &addr);
    void finished();
    void HandleSelectionChange(QModelIndex current, QModelIndex previous);
    void onSettingsChanged();
    void UpdateServers();

private slots:
    void customMenuRequested(QPoint pos);
    void dm();
    void onAddTo();
    void onModelDataChanged(QModelIndex, QModelIndex);
    void onRemoveFrom();
    void play();
    void requestChangeServerSettings();

};

#endif // SERVERTABLEWIDGET_H
