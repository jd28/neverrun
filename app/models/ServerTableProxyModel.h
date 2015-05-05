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

#ifndef SERVERTABLEPROXYMODEL_H
#define SERVERTABLEPROXYMODEL_H

#include <QDebug>
#include <QModelIndex>
#include <QObject>
#include <QStringList>
#include <QSortFilterProxyModel>
#include <QVariant>

#include "servertablemodel.h"

class ServerTableProxyModel : public QSortFilterProxyModel {
    int current_room_;
public:
    ServerTableProxyModel(QObject *parent)
        : QSortFilterProxyModel(parent)
    {
        current_room_ = -1;
    }

    void reset() { resetInternalData(); }

    void setCurrentRoom(int room) {
        current_room_ = room;
        invalidateFilter();
    }

    bool lessThan(const QModelIndex &left,
                  const QModelIndex &right) const
    {
        QVariant leftData = sourceModel()->data(left);
        QVariant rightData = sourceModel()->data(right);

        if (left.column() == ServerTableModel::COLUMN_SERVER_NAME || left.column() == ServerTableModel::COLUMN_MODULE_NAME) {
            return leftData.toString().compare(rightData.toString(), Qt::CaseInsensitive) < 0;
        }
        else if (left.column() == ServerTableModel::COLUMN_PASSWORD) {
            QVariant l = sourceModel()->data(left, Qt::UserRole + 1);
            QVariant r = sourceModel()->data(right, Qt::UserRole + 1);
            return l.isValid() && !l.toBool() && r.toBool();
            //return !l;
        }
        else if (left.column() == ServerTableModel::COLUMN_PLAYER_COUNT) {
            QVariant l = sourceModel()->data(left, Qt::UserRole + 1);
            QVariant r = sourceModel()->data(right, Qt::UserRole + 1);
            return l.toInt() < r.toInt();
        }
        else if (left.column() == ServerTableModel::COLUMN_PING) {
            if (leftData.toInt() == -1) { return false; }
            if (rightData.toInt() == -1) { return true; }
            return leftData.toInt() < rightData.toInt();
        }
        else if (leftData.type() == QVariant::Int) {
            return leftData.toInt() < rightData.toInt();
        }
        return leftData < rightData;
    }

    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const {
        QModelIndex i = sourceModel()->index(source_row, ServerTableModel::COLUMN_SERVER_NAME, source_parent);
        bool online = sourceModel()->data(i, Qt::UserRole + 5).toBool();
        QString address = sourceModel()->data(i, Qt::UserRole + 3).toString();
        QString ip = sourceModel()->data(i, Qt::UserRole + 6).toString();
        if(blacklist_.contains(ip)) {
            return false;
        }
        if (current_room_ == -3) {
            return ip_filter_.contains(address);
        }
        else if (current_room_ == -2) {
            return ip_filter_.contains(address) && online;
        }
        else if (current_room_ == -1) {
            return online;
        }
        return online && sourceModel()->data(sourceModel()->index(source_row, ServerTableModel::COLUMN_SERVER_NAME, source_parent),
                                             Qt::UserRole + 1).toInt() == current_room_;
    }

    void resetFilter() { invalidateFilter(); }

    void SetServerAddressFilter(const QStringList& ips);
    void SetServerBlacklist(const QStringList& ips);
private:
    QStringList ip_filter_;
    QStringList blacklist_;

};


#endif // SERVERTABLEPROXYMODEL_H
