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

#include <QComboBox>
#include <QDebug>
#include <QFont>
#include <QGridLayout>
#include <QHeaderView>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QPainter>
#include <QPushButton>
#include <QSettings>
#include <QSpacerItem>
#include <QStyledItemDelegate>
#include <QToolBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

#include "servercategorywidget.h"
#include "../options.h"
#include "../delegates/TableSeperatorDelegate.h"
#include "../util.h"

void ServerCategoryWidget::customMenuRequested(QPoint pos) {
    auto it = itemAt(pos);

    QMenu menu(this);
    QAction *act = menu.addAction("Add Category");
    connect(act, &QAction::triggered, [this] { emit requestAddCategory(); });

    if( it && it->data(Qt::UserRole + 2).toBool()) {
        act = menu.addAction("Remove Category");
        connect(act, &QAction::triggered, this, &ServerCategoryWidget::onRemoveCategory);
    }
    menu.exec(viewport()->mapToGlobal(pos));
}

void ServerCategoryWidget::onRemoveCategory() {
    auto its = selectionModel()->selectedRows();
    foreach(const QModelIndex& it, its) {
        auto i = this->item(it.row(), 0);
        if (!i->data(Qt::UserRole + 2).toBool()) { continue; }
        options_->removeServerCategory(i->text());
        removeRow(it.row());
    }
}

ServerCategoryWidget::ServerCategoryWidget(Options *options, QWidget *parent) :
    options_(options),
    QTableWidget(15, 1, parent)
{
    setColumnCount(1);
    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPixelSize(20);
    setFont(font);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customMenuRequested(QPoint)));

    QTableWidgetItem *item;
    int row = 0;

#define add_row(name, user, seperator) \
    item = new QTableWidgetItem(name); \
    item->setData(Qt::UserRole+1, seperator); \
    item->setData(Qt::UserRole+2, user); \
    item->setFlags(item->flags() ^ Qt::ItemIsEditable); \
    setItem(0, row++, item)

    add_row("All", false, true);
    add_row("Action", false, false);
    add_row("Arena", false, false);
    add_row("Melee", false, false);
    add_row("PW Action", false, false);
    add_row("PW Story", false, false);
    add_row("Role Play", false, false);
    add_row("Social", false, false);
    add_row("Solo", false, false);
    add_row("Story", false, false);
    add_row("Story Lite", false, false);
    add_row("Team", false, false);
    add_row("Tech Support", false, true);
    add_row("History", false, true);
    add_row("Favorites", false, true);

    QStringList cs = options_->getServerCategories();
    cs.sort(Qt::CaseInsensitive);

    setRowCount(rowCount() + cs.size() - 2);
    foreach(const QString& c, cs) {
        if (c == "Favorites" || c == "History")
            continue;

        add_row(c, true, false);
    }

    verticalHeader()->hide();
    setHorizontalHeaderLabels(QStringList() << "Categories");
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    setShowGrid(false);
    setItemDelegateForColumn(0, new SeperatorDelegate(this));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    connect(selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(changeServerList()));

#undef add_row
}

void ServerCategoryWidget::selectAllCategory() {
    selectRow(0);
}

void ServerCategoryWidget::addCategory(const QString &cat) {
    if(options_->addServerCategory(cat)) {
        setRowCount(rowCount() + 1);

        int row = -1;
        for (int i = 0; i < rowCount() - 1; ++i) {
            QTableWidgetItem *it = item(i, 0);
            if (!it) { break; }
            if (!it->data(Qt::UserRole + 2).toBool()) { continue; }
            if(it->text().compare(cat, Qt::CaseInsensitive) > 0) {
                for(int j = rowCount() - 1; j >= i; --j) {
                    setItem(j+1, 0, takeItem(j, 0));
                    row = j;
                }
                break;
            }
        }

        QTableWidgetItem *item = new QTableWidgetItem(cat);
        item->setData(Qt::UserRole+2, true);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        if(row == -1) {
            setItem(rowCount() - 1, 0, item);
        }
        else {
            setItem(row, 0, item);
        }

    }
}

void ServerCategoryWidget::changeServerList()
{
    foreach(QTableWidgetItem *x, selectedItems()) {
        QStringList ips;
        QString cat;
        if (x->text() == "All") {
            emit loadAllServers(-1);
        }
        else {
            int res = RoomToGameType(x->text());
            if(res == -1) {
                cat = x->text();
                ips = options_->getCategoryIPs(x->text());
                if(cat == "History") {
                    emit loadAllServers(-3);
                }
                else {
                    emit loadAllServers(-2);
                }
            }
            else {
                emit loadAllServers(res);
            }
        }
        emit updateFilter(ips, cat);
    }
}
