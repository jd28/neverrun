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

#ifndef LISTSELECTIONDIALOG_H
#define LISTSELECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class ListSelectionDialog;
}

class ListSelectionDialog : public QDialog {
    Q_OBJECT

public:
    enum Mode {
        MODE_NONE,
        MODE_ADD_TO,
        MODE_REMOVE_FROM,
        MODE_REMOVE_CATEGORY,
        MODE_REMOVE_USERNAME
    };

private:
    Mode mode_;
    Ui::ListSelectionDialog *ui;

public:
    explicit ListSelectionDialog(QWidget *parent = 0);
    ~ListSelectionDialog();

    Mode getMode() const { return mode_; }
    QStringList getSelectedItems() const;

    void setLabel(const QString& name);
    void setListItems(const QStringList& list);
    void setMode(Mode mode) { mode_ = mode; }
};

#endif // LISTSELECTIONDIALOG_H
