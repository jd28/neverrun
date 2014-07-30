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
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QToolBox>
#include <QVBoxLayout>

#include "modulecategorywidget.h"
#include "../options.h"
#include "../delegates/TableSeperatorDelegate.h"

void ModuleCategoryWidget::customMenuRequested(QPoint pos) {
    auto it = itemAt(pos);
    if( !it || !it->data(Qt::UserRole + 2).toBool())
        return;

    QMenu menu(this);

    auto act = new QAction("Remove Category", this);
    connect(act, SIGNAL(triggered()), this, SLOT(onRemoveCategory()));
    menu.addAction(act);

    menu.exec(viewport()->mapToGlobal(pos));
}

void ModuleCategoryWidget::onRemoveCategory() {
    auto its = selectionModel()->selectedRows();
    foreach(const QModelIndex& it, its) {
        auto i = this->item(it.row(), 0);
        if (!i->data(Qt::UserRole + 2).toBool()) { continue; }
        options_->removeModuleCategory(i->text());
        removeRow(it.row());
    }
}

ModuleCategoryWidget::ModuleCategoryWidget(Options *options, QWidget *parent) :
    options_(options),
    QTableWidget(2, 1, parent)
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
    add_row("Favorites", false, true);

    QStringList cs = options_->getModuleCategories();
    cs.sort(Qt::CaseInsensitive);

    setRowCount(rowCount() + cs.size() - 1);
    foreach(const QString& c, cs) {
        if (c == "Favorites")
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
            SLOT(ChangeModuleList()));


#undef add_row
}

void ModuleCategoryWidget::SelectAll() {
    selectRow(0);
}

void ModuleCategoryWidget::addCategory(const QString &cat) {
    if(options_->addModuleCategory(cat)) {
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

void ModuleCategoryWidget::ChangeModuleList()
{
    foreach(QTableWidgetItem *x, selectedItems()) {
        QStringList ips;
        if (x->text() == "All") { emit LoadModules(-1); return; }

        ips = options_->getCategoryModules(x->text());
        emit LoadModules(-2);
        emit UpdateFilter(ips);
    }
}
