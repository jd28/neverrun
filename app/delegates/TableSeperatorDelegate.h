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

#ifndef TABLESEPERATORDELEGATE_H
#define TABLESEPERATORDELEGATE_H

class SeperatorDelegate : public QStyledItemDelegate {
public:
    SeperatorDelegate(QObject *parent)
        : QStyledItemDelegate(parent)
    {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        QStyledItemDelegate::paint(painter, option, index);

        QVariant isSeparator = index.data(Qt::UserRole + 1);
        if (isSeparator.isValid() && isSeparator.toBool())
        {
            QRect rct = option.rect;
            rct.setY(rct.bottom());
            painter->fillRect(rct, QColor::fromRgb(qRgb(108,108,108)));
        }
    }
};

#endif // TABLESEPERATORDELEGATE_H
