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

#ifndef SERVERTABLEMODEL_H
#define SERVERTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "../server.h"

class ServerTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnNames {
        COLUMN_PASSWORD,
        COLUMN_SERVER_NAME,
        COLUMN_MODULE_NAME,
        COLUMN_PLAYER_COUNT,
        COLUMN_ADDRESS,
        COLUMN_LEVELS,
        COLUMN_ELC,
        COLUMN_ILR,
        COLUMN_ONE_PARTY,
        COLUMN_LOCAL_VAULT
    };

    enum UserRoles {
        USER_ROLE_GAMETYPE = Qt::UserRole + 1,
        USER_ROLE_WEBPAGE = Qt::UserRole + 2
    };

    explicit ServerTableModel(QObject *parent = 0);
    ServerTableModel(std::vector<Server> servers, QObject *parent = 0);

    void SetServers(std::vector<Server> servers) {
        beginInsertRows(QModelIndex(), 0, servers.size() - 1);
        servers_ = std::move(servers);
        endInsertRows();
        //emit(dataChanged(index(0,0), index(servers_.size(), 9)));
    }
    void UpdateSevers(const std::vector<Server>& servers);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
   // bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());

    Server getServer(QModelIndex index) const;
public slots:
    void addServer(const QString &addr);
private:
    std::vector<Server> servers_;
    QMap<QString, int> server_map_;
};

#endif // SERVERTABLEMODEL_H
