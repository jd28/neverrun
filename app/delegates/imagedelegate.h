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

#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H

class ImageDelegate : public QItemDelegate {
    QImage m_image;
public:
    ImageDelegate(QObject *parent=0) : QItemDelegate(parent){
        QImage newImage(":/qss_icons/rc/padlock_closed.png");
        m_image = newImage.copy();
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        QItemDelegate::paint(painter, option, index);
        if ( index.model()->data(index, Qt::UserRole + 1).toBool() )
            painter->drawImage(
                        (option.rect.width() - m_image.width()) / 2,
                        option.rect.y() + (option.rect.height() - m_image.height()),
                        m_image);
    }
};

#endif // IMAGEDELEGATE_H
