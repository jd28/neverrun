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

#ifndef USERNAMEBUTTON_H
#define USERNAMEBUTTON_H

#include <QPushButton>

class QMenu;
class TextBoxDialog;
class ListSelectionDialog;
class Options;

class UserNameButton : public QPushButton
{
    Q_OBJECT

    Options *options_;
    QMenu *name_menu_;
    TextBoxDialog *add_user_;
    ListSelectionDialog *list_selection_dlg_;

private slots:
    void addName();
    void changeUserName(QAction *action);
    void onRemoveUserNames();
    void showAddUserName();
    void showRemoveUserNames();

protected:
    virtual void paintEvent( QPaintEvent * );

public:
    UserNameButton(Options *options, const QString &current, const QStringList &all, QWidget *parent = 0);

signals:
    void addUserName(QString username);
    void userNameChanged(QString username);
};

#endif // USERNAMEBUTTON_H
