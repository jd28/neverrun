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

#ifndef MODULETABLEPROXYMODEL_H
#define MODULETABLEPROXYMODEL_H

#include <QSortFilterProxyModel>

class ModuleTableProxyModel : public QSortFilterProxyModel {
    int current_room_;
public:
    ModuleTableProxyModel(QObject *parent)
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

        if (left.column() == 0) {
            return leftData.toString().compare(rightData.toString(), Qt::CaseInsensitive) < 0;
        }
        return leftData < rightData;
    }

    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const {
        if(current_room_ == -1) { return true; }
        else if (current_room_ == -2) {
            if ( mod_filter_.size() > 0 ) {
                QModelIndex i = sourceModel()->index(source_row, 0, source_parent);
                return mod_filter_.contains(sourceModel()->data(i).toString());
            }
            return false;
        }
        return true;
    }

    void setModuleFilter(const QStringList& mods);
private:
    QStringList mod_filter_;

};

#endif // MODULETABLEPROXYMODEL_H
