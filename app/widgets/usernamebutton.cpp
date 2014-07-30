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
#include <QMenu>
#include <QSignalMapper>

#include "setdmpassworddialog.h"
#include "listselectiondialog.h"
#include "../options.h"
#include "usernamebutton.h"

UserNameButton::UserNameButton(Options *options, const QString& current, const QStringList &all, QWidget *parent)
    : QToolButton(parent)
    , options_(options)
{

    setText(current);
    name_menu_ = new QMenu(this);
    for (int i = 0; i < all.size(); ++i) {
        name_menu_->addAction(all[i]);
    }
    connect(name_menu_, SIGNAL(triggered(QAction*)),
            this, SLOT(changeUserName(QAction*)));

    //setStyleSheet("font-size: 20px; margin: 5px;");
    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPixelSize(15);
    setFont(font);
    setMinimumHeight(42);

    list_selection_dlg_ = new ListSelectionDialog(this);
    connect(list_selection_dlg_, SIGNAL(accepted()), SLOT(onRemoveUserNames()));

    name_menu_->addSeparator();

    auto act = new QAction("Add User Name", this);
    connect(act, SIGNAL(triggered()), SLOT(showAddUserName()));
    name_menu_->addAction(act);
    act->setProperty("NAME_LABEL_NOT_NAME", true);

    act = new QAction("Remove User Name(s)", this);
    connect(act, SIGNAL(triggered()), SLOT(showRemoveUserNames()));
    act->setProperty("NAME_LABEL_NOT_NAME", true);

    name_menu_->addAction(act);
    setPopupMode(QToolButton::InstantPopup);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    add_user_ = new TextBoxDialog(TextBoxDialog::MODE_USER_NAME, this);
    connect(add_user_, SIGNAL(accepted()), SLOT(addUserName()));

    setMenu(name_menu_);
}

void UserNameButton::showAddUserName() {
    add_user_->show();
}

void UserNameButton::showRemoveUserNames() {
    QStringList ls = options_->getUserNames();
    ls.removeAll(options_->getCurrentUserName());

    list_selection_dlg_->setListItems(ls);
    list_selection_dlg_->setMode(ListSelectionDialog::MODE_REMOVE_USERNAME);
    list_selection_dlg_->setLabel("Remove User Name(s)");
    list_selection_dlg_->show();
}

void UserNameButton::onRemoveUserNames() {
    QStringList ls = list_selection_dlg_->getSelectedItems();
    foreach(const QString& l, ls) {
        options_->removeUserName(l);
    }
    foreach(QAction *a, name_menu_->actions()) {
        if (ls.contains(a->text())) {
            name_menu_->removeAction(a);
        }
    }
}

void UserNameButton::addUserName() {
    auto u = add_user_->getText();


    QAction *before;
    if(!name_menu_->actions().empty())  {
        foreach(QAction *a, name_menu_->actions()) {
            if( u.compare(a->text(), Qt::CaseInsensitive) < 0 || a->text() == "") {
                before = a;
                break;
            }
            if(u == a->text()) { return; }
        }
    }

    if(before) {
        auto act = new QAction(u, this);
        name_menu_->insertAction(before, act);
    }
    emit addUserName(u);

    setText(u);
    emit userNameChanged(u);
}


void UserNameButton::changeUserName(QAction *action) {
    if (action->property("NAME_LABEL_NOT_NAME").toBool()) {
        return;
    }

    setText(action->text());
    emit userNameChanged(action->text());
}
