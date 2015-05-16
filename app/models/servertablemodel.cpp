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
#include <QUdpSocket>
#include <QTimer>
#include <QVariant>

#include "servertablemodel.h"
#include "../widgets/servertablewidget.h"
#include "../util.h"
#include "../server.h"

ServerTableModel::ServerTableModel(QObject *parent)
    : QAbstractTableModel(parent) {
}

ServerTableModel::ServerTableModel(std::vector<Server> servers, QObject *parent)
    : QAbstractTableModel(parent)
    , servers_(std::move(servers))
    , current_update_index_(0) {

    for(size_t i = 0; i < servers_.size(); ++i){
        server_map_.insert(servers_[i].address + QString::number(servers_[i].port), i);
    }
    last_active_ = 0;
}

void ServerTableModel::UpdateSevers(const std::vector<Server> &servers) {
    bool update = false;
    for(size_t i = 0; i < servers.size(); ++i) {
        auto it = server_map_.find(servers[i].address + QString::number(servers[i].port));
        if(it == server_map_.end()) {
            qDebug() << "Adding server: " << servers[i].address + QString::number(servers[i].port)
                     << " " << servers[i].module_name << " at " << servers_.size();

            server_map_.insert(servers[i].address + QString::number(servers[i].port), servers_.size());
            insertRows(servers_.size(), 1);
            servers_[servers_.size()-1] = servers[i];
            update = true;
        }
    }
    if(update)
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

void ServerTableModel::readDatagrams() {
    while (udp_socket_->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udp_socket_->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        udp_socket_->readDatagram(datagram.data(), datagram.size(),
                                  &sender, &senderPort);

        auto it = server_map_.find(sender.toString() + QString::number(senderPort));
        if  (it == server_map_.end()) { continue; }

        size_t idx = it.value();
        servers_[idx].last_contact = getTickCount();
        servers_[idx].addPing(servers_[idx].last_contact - servers_[idx].last_query);

        servers_[idx].online = true;

        if (readPacket(datagram, servers_[idx]))
            emit dataChanged(QModelIndex(), QModelIndex());
    }
}

int ServerTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return servers_.size();
}

int ServerTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 5;
}

QVariant ServerTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if ((unsigned)index.row() >= servers_.size())
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
        return !s.isOffline();
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
        case COLUMN_PING: return s.getPing();
        }
    }
    else if (Qt::TextAlignmentRole == role && (index.column() == COLUMN_PLAYER_COUNT
                                               || index.column() == COLUMN_LEVELS
                                               || index.column() == COLUMN_PING)) {
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
        case COLUMN_PING: return "Ping";
        }
    }

    return QVariant();
}

Qt::ItemFlags ServerTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index);
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

void ServerTableModel::sweepOfflineServers() {
    bool update = false;

    // I do not like this at all...
    for(auto it = servers_.begin(); it != servers_.end(); ++it) {
        uint64_t tick = getTickCount();
        Server& s = *it;
        if ( s.online
             && last_active_ != 0
             && tick - last_active_ > 20000
             && tick - s.last_contact > 20000 ) {
                s.reset();
                s.server_name = (*it).address + ":" + QString::number(s.port);
                s.module_name = "Offline";
                update = true;
        }
    }

    if(update)
        emit dataChanged(QModelIndex(), QModelIndex());

}

void ServerTableModel::setServerAddressFilter(const QStringList &ips) {
    ip_filter_ = ips;
}

void ServerTableModel::bindUpdSocket(int port) {
    udp_socket_ = new QUdpSocket(this);
    bool res = udp_socket_->bind(port);
    if (!res) { return; }
    connect(udp_socket_, SIGNAL(readyRead()),SLOT(readDatagrams()));
}

void ServerTableModel::requestUpdate(const Server& s) {
    udp_socket_->writeDatagram(bnxi_, QHostAddress(s.address), s.port);
    if ( (s.messages_received & SERVER_MESSAGES_RECIEVED_BNDR) == 0 ) {
        udp_socket_->writeDatagram(bnds_, QHostAddress(s.address), s.port);
    }

    if ( (s.messages_received & SERVER_MESSAGES_RECIEVED_BNER) == 0 ) {
        udp_socket_->writeDatagram(bnes_, QHostAddress(s.address), s.port);
    }
}

void ServerTableModel::requestUpdates() {
    if ( current_update_index_ == servers_.size() )
        current_update_index_ = 0;
    Server& s = servers_[current_update_index_];
    QString add = s.address + ":" + QString::number(s.port);
    if(ip_filter_.size() > 0 && !ip_filter_.contains(add)) {
        //qDebug() << "Skipping: " << add << " not on IP Filter List";
    }
    else if (current_room_ >= 0 && current_room_ != s.gametype) {
        //qDebug() << "Skipping: " << add << " not correct gametype.";
    }
    else {
        servers_[current_update_index_].last_query = getTickCount();
        requestUpdate(servers_[current_update_index_]);
    }

    ++current_update_index_;
}
