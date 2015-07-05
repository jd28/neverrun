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

#ifndef SETDMPASSWORDDIALOG_H
#define SETDMPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class SetDMPasswordDialog;
}

class TextBoxDialog : public QDialog
{
    Q_OBJECT

public:
    enum Mode {
        MODE_USER_NAME,
        MODE_DM_PASSWORD,
        MODE_PLAYER_PASSWORD,
        MODE_ADD_CATEGORY
    };

private:
    Mode mode_;
    Ui::SetDMPasswordDialog *ui;

public:
    explicit TextBoxDialog(QWidget *parent = 0);
    TextBoxDialog(Mode mode, QWidget *parent = 0);
    ~TextBoxDialog();

    Mode getEntryMode() const;
    QString getText() const;
    void setEntryMode(Mode mode);
    void setText(const QString& text);

};

#endif // SETDMPASSWORDDIALOG_H
