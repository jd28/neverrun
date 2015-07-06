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

#include <QAction>
#include <QDebug>
#include <QHeaderView>
#include <QItemDelegate>
#include <QHostAddress>
#include <QMenu>
#include <QPainter>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QtConcurrent/QtConcurrentRun>
#include <QTimer>
#include <QUdpSocket>
#include <QTableView>

#include "moduletablewidget.h"
#include "../options.h"

ModuleTableWidget::ModuleTableWidget(Options *options, QWidget *parent)
    : QTableView(parent)
    , model_(new ModuleTableModel(options, this))
    , proxy_model_(new ModuleTableProxyModel(this))
    , options_(options)
{

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &ModuleTableWidget::customContextMenuRequested,
            this, &ModuleTableWidget::customMenuRequested);

    proxy_model_->setSourceModel(model_);
    proxy_model_->setDynamicSortFilter(true);
    setModel(proxy_model_);
    horizontalHeader()->setSortIndicatorShown(true);
    horizontalHeader()->setSectionsClickable(true);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setSortingEnabled(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    verticalHeader()->setVisible(false);
    horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);

}

void ModuleTableWidget::setModuleFilter(const QStringList &mods, const QString& cat) {
    current_cat_ = cat;
    proxy_model_->setModuleFilter(mods);
}

void ModuleTableWidget::onRemoveFromCat() {
    auto selections = selectedIndexes();
    if (selections.size() == 0) { return; }
    auto idx = model()->index(selections[0].row(), 0);
    QString mod = model()->data(idx).toString();
    options_->removeModuleFromCategory(current_cat_, mod);
    setModuleFilter(options_->getCategoryModules(current_cat_), current_cat_);
}

void ModuleTableWidget::customMenuRequested(QPoint pos) {
    QModelIndex index = indexAt(pos);
    if (!index.isValid()) { return; }

    QMenu menu(this);

    QAction *act = nullptr;

    act = menu.addAction("Play");
    connect(act, &QAction::triggered, [this] { emit play(); });

    act = menu.addAction("Edit");
    connect(act, &QAction::triggered, [this] { emit edit(); });

    //Loading DM module not supported currently
    //menu.addAction("DM");
    //connect(act, &QAction::triggered, this, &ModuleTableWidget::dm);

    menu.addSeparator();

    act = menu.addAction("Add to...");
    connect(act, &QAction::triggered, [this] { emit requestAddTo(); });
    act = menu.addAction("Remove from...");
    connect(act, &QAction::triggered, [this] { emit requestRemoveFrom(); });

    if(current_cat_.size() > 0) {
        act = menu.addAction("Remove from");
        connect(act, &QAction::triggered, this, &ModuleTableWidget::onRemoveFromCat);
    }

    menu.exec(viewport()->mapToGlobal(pos));
}

void ModuleTableWidget::loadModules(int room) {
    if (model_->rowCount(QModelIndex()) > 0) {
        ModuleTableProxyModel* pm = reinterpret_cast<ModuleTableProxyModel*>(model());
        pm->setCurrentRoom(room);
    }
}

