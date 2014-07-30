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

#include "listselectiondialog.h"
#include "ui_listselectiondialog.h"

ListSelectionDialog::ListSelectionDialog(QWidget *parent) :
    QDialog(parent),
    mode_(MODE_NONE),
    ui(new Ui::ListSelectionDialog)
{
    setWindowFlags(Qt::SplashScreen);
    setModal(true);
    ui->setupUi(this);
}

ListSelectionDialog::~ListSelectionDialog()
{
    delete ui;
}

QStringList ListSelectionDialog::getSelectedItems() const {
    QStringList items;
    foreach(const QListWidgetItem *i, ui->item_list->selectedItems()) {
        items << i->text();
    }
    return items;
}

void ListSelectionDialog::setListItems(const QStringList &list) {
    ui->item_list->clear();
    ui->item_list->addItems(list);
}

void ListSelectionDialog::setLabel(const QString &name) {
    ui->groupBox->setTitle(name);
}
