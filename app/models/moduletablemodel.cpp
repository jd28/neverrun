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

#include <QDebug>
#include <QDir>
#include <QDirIterator>

#include "moduletablemodel.h"
#include "../options.h"

ModuleTableModel::ModuleTableModel(Options *options, QObject *parent)
    : QAbstractTableModel(parent)
    , options_(options)
{
    auto path = QDir(QDir::cleanPath(options_->m_NWN_path + QDir::separator() + "modules"));
    QDirIterator it(path, QDirIterator::FollowSymlinks);
    while (it.hasNext()) {
        QString next = it.next();
        if(next.endsWith(".mod", Qt::CaseInsensitive))
            modules_.push_back(QFileInfo(next).fileName());
    }

}

int ModuleTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return modules_.size();
}

int ModuleTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant ModuleTableModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= modules_.size())
        return QVariant();

    if (role == Qt::FontRole) {
        QFont font;
        font.setFamily(font.defaultFamily());
        font.setPixelSize(20);
        return font;
    }
    else if (role == Qt::DisplayRole) {
        switch (index.column()) {
        default: return QVariant();
        case COLUMN_MODULE_FILE: return modules_[index.row()];
        }
    }

    return QVariant();
}

QVariant ModuleTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        default: return QVariant();
        case COLUMN_MODULE_FILE: return "File";
        }
    }

    return QVariant();

}

Qt::ItemFlags ModuleTableModel::flags(const QModelIndex &index) const {
    return QAbstractTableModel::flags(index);
}

bool ModuleTableModel::insertRows(int position, int rows, const QModelIndex &index) {
    Q_UNUSED(index);
    Q_UNUSED(position);
    Q_UNUSED(rows);
    return false;
}

bool ModuleTableModel::removeRows(int position, int rows, const QModelIndex &index) {
    Q_UNUSED(index);
    Q_UNUSED(position);
    Q_UNUSED(rows);
    return false;
}
