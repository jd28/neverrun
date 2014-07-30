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

ModuleTableWidget::ModuleTableWidget(Options *options, QWidget *parent)
    : QTableView(parent)
    , model_(new ModuleTableModel(options, this))
    , proxy_model_(new ModuleTableProxyModel(this))
{

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(customMenuRequested(QPoint)));

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

    act_add_to_ = new QAction("Add to...", this);
    connect(act_add_to_, SIGNAL(triggered()), SLOT(onAddTo()));
    act_remove_from_ = new QAction("Remove From...", this);
    connect(act_remove_from_, SIGNAL(triggered()), SLOT(onRemoveFrom()));

}

void ModuleTableWidget::setModuleFilter(const QStringList &mods) {
    proxy_model_->setModuleFilter(mods);
}

void ModuleTableWidget::onAddTo() {
    emit requestAddTo();
}

void ModuleTableWidget::onRemoveFrom() {
    emit requestRemoveFrom();
}

const ModuleTableModel *ModuleTableWidget::getModuleTableModel() const {
    return model_;
}

void ModuleTableWidget::HandleSelectionChange(QModelIndex current, QModelIndex previous) {
    Q_UNUSED(current);
    Q_UNUSED(previous);
}

void ModuleTableWidget::customMenuRequested(QPoint pos) {
    QModelIndex index = indexAt(pos);
    if (!index.isValid()) { return; }

    QMenu menu(this);

    auto act = new QAction("Play", this);
    connect(act, SIGNAL(triggered()), this, SLOT(play()));
    menu.addAction(act);


    act = new QAction("DM", this);
    connect(act, SIGNAL(triggered()), this, SLOT(dm()));
    menu.addAction(act);

    menu.addSeparator();

    menu.addAction(act_add_to_);
    menu.addAction(act_remove_from_);

    menu.addSeparator();

    menu.exec(viewport()->mapToGlobal(pos));
}

void ModuleTableWidget::loadModules(int room) {
    if (model_->rowCount(QModelIndex()) > 0) {
        ModuleTableProxyModel* pm = reinterpret_cast<ModuleTableProxyModel*>(model());
        pm->setCurrentRoom(room);
    }
}
