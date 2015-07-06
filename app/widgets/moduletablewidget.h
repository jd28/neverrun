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

#ifndef MODULETABLEWIDGET_H
#define MODULETABLEWIDGET_H

#include <QDebug>
#include <QFutureWatcher>
#include <QHeaderView>
#include <QPainter>
#include <QTableView>

#include "../models/moduletablemodel.h"
#include "../models/moduletableproxymodel.h"

class Options;
class TextBoxDialog;

class ModuleTableWidget : public QTableView {
    Q_OBJECT

    QString                current_cat_;
    ModuleTableModel      *model_;
    ModuleTableProxyModel *proxy_model_;
    Options               *options_;

private slots:
    void customMenuRequested(QPoint pos);
    void onRemoveFromCat();

public:
    explicit ModuleTableWidget(Options *options, QWidget *parent = 0);

    void loadModules(int room);
    void setModuleFilter(const QStringList &mods, const QString &cat);

signals:
    void edit();
    void play();
    void requestAddTo();
    void requestRemoveFrom();
};

#endif // MODULETABLEWIDGET_H
