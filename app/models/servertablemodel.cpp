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

#include <QColor>
#include <QDebug>
#include <QHeaderView>
#include <QVariant>

#include "servertablemodel.h"
#include "../widgets/servertablewidget.h"

ServerTableModel::ServerTableModel(QObject *parent)
    : QAbstractTableModel(parent) {
}

ServerTableModel::ServerTableModel(std::vector<Server> servers, QObject *parent)
    : QAbstractTableModel(parent)
    , servers_(std::move(servers)) {

    for(int i = 0; i < servers_.size(); ++i){
        server_map_.insert(servers_[i].address + QString::number(servers_[i].port), i);
    }

}

void ServerTableModel::UpdateSevers(const std::vector<Server> &servers) {
    // Set everything to offline each update, then turn them back on if they are in fact online.
    for(auto it = servers_.begin(); it != servers_.end(); ++it) { (*it).online = false; }

    for(int i = 0; i < servers.size(); ++i) {
        auto it = server_map_.find(servers[i].address + QString::number(servers[i].port));
        if(it != server_map_.end()) {
            if( servers[i].online && servers_[it.value()].module_name == "Offline") {
                qDebug() << "A server has come online: " << servers[i].module_name;
                // Replace the whole server entry
                servers_[it.value()] = servers[i];
            }

            servers_[it.value()].cur_players = servers[i].cur_players;
            servers_[it.value()].online = servers[i].online;
        }
        else { // If it's not in the map we got a new server...
            qDebug() << "Adding server: " << servers[i].address + QString::number(servers[i].port)
                     << " " << servers[i].module_name << " at " << servers_.size();

            server_map_.insert(servers[i].address + QString::number(servers[i].port), servers_.size());
            insertRows(servers_.size(), 1);
            servers_[servers_.size()-1] = servers[i];
        }
    }
    emit dataChanged(QModelIndex(), QModelIndex());
}

void ServerTableModel::addServer(const QString& addr) {
    QRegExp rx("(\\:)");
    QStringList query = addr.split(rx);
    Q_ASSERT(query.size() == 2);
    Server serv;
    serv.address = query[0];
    serv.port = query[1].toInt();
    serv.module_name = "Offline";
    serv.server_name = addr;
    serv.online = false;

    auto it = server_map_.find(serv.address + QString::number(serv.port));
    if(it != server_map_.end()) {
        qDebug() << "Address Found...";
        return;
    }

    qDebug() << "Attempting to add server: " << addr;
    qDebug() << servers_.size();

    server_map_.insert(serv.address + QString::number(serv.port), servers_.size());
    insertRows(servers_.size(), 1);
    servers_[servers_.size()-1] = serv;
    emit dataChanged(QModelIndex(), QModelIndex());
}

int ServerTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return servers_.size();
}

int ServerTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 4;
}

QVariant ServerTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= servers_.size() || index.row() < 0)
        return QVariant();

    const Server &s = servers_[index.row()];

    if (role == Qt::FontRole) {
        QFont font;
        font.setFamily(font.defaultFamily());
        font.setPixelSize(20);
        return font;
    }
    else if (role == Qt::UserRole + 1) {
        switch(index.column()) {
        default: return QVariant();
        case COLUMN_SERVER_NAME: return s.gametype;
        case COLUMN_PASSWORD: return s.password;
        case COLUMN_PLAYER_COUNT: return s.cur_players;
        }
    }
    else if (role == USER_ROLE_WEBPAGE && index.column() == COLUMN_SERVER_NAME) {
        return s.homepage;
    }
    else if (role == Qt::UserRole + 3 && index.column() == COLUMN_SERVER_NAME) {
        return s.address + ":" + QString::number(s.port);
    }
    else if (role == Qt::UserRole + 4 && index.column() == COLUMN_SERVER_NAME) {
        return s.toStringList();
    }
    else if (role == Qt::UserRole + 5 && index.column() == COLUMN_SERVER_NAME) {
        return s.online;
    }
    else if (role == Qt::UserRole + 6 && index.column() == COLUMN_SERVER_NAME) {
        return s.address;
    }
    else if (role == Qt::UserRole + 7 && index.column() == COLUMN_SERVER_NAME) {
        return s.port;
    }

    else if (role == Qt::DisplayRole) {
        switch (index.column()) {
        default: return QVariant();
        case COLUMN_SERVER_NAME: return s.server_name;
        case COLUMN_MODULE_NAME: return s.module_name;
        case COLUMN_PLAYER_COUNT: return QString::number(s.cur_players) + "/" + QString::number(s.max_players);
        }
    }
    else if (Qt::TextAlignmentRole == role && (index.column() == COLUMN_PLAYER_COUNT
                                               || index.column() == COLUMN_LEVELS)) {
        return Qt::AlignCenter;
    }

    return QVariant();
}

QVariant ServerTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        default: return QVariant();
        case COLUMN_SERVER_NAME: return "Server Name";
        case COLUMN_MODULE_NAME: return "Module Name";
        case COLUMN_PLAYER_COUNT: return "Players";
        }
    }

    return QVariant();
}

Qt::ItemFlags ServerTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index);
}

Server ServerTableModel::getServer(QModelIndex index) const {
    if (index.row() >= servers_.size()) { return Server(); }
    return servers_[index.row()];
}

bool ServerTableModel::insertRows(int position, int rows, const QModelIndex &index) {
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);
    if(position == servers_.size()){
        for (int i = 0; i < rows; ++i)
            servers_.push_back(Server());
    }
    else {
        servers_.insert(servers_.begin() + position, Server());
    }
    endInsertRows();
    return true;
}

bool ServerTableModel::removeRows(int position, int rows, const QModelIndex &index) {
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    servers_.erase(std::begin(servers_) + position, std::begin(servers_) + position+rows-1);
    endRemoveRows();
    return true;
}
