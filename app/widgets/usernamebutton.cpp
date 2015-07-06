#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QStyleOptionButton>
#include <QStylePainter>

#include "setdmpassworddialog.h"
#include "listselectiondialog.h"
#include "../options.h"
#include "usernamebutton.h"

UserNameButton::UserNameButton(Options *options, const QString& current, const QStringList &all, QWidget *parent)
    : QPushButton(parent)
    , options_(options)
{

    setText(current);
    setStyleSheet("font-size: 30px;");
    setMinimumWidth( QFontMetrics(font()).boundingRect(current).width() );
    name_menu_ = new QMenu(this);
    name_menu_->setStyleSheet("font-size: 16px; text-align: center;");
    for (int i = 0; i < all.size(); ++i) {
        name_menu_->addAction(all[i]);
    }
    connect(name_menu_, &QMenu::triggered, this, &UserNameButton::changeUserName);

    list_selection_dlg_ = new ListSelectionDialog(this);
    connect(list_selection_dlg_, &ListSelectionDialog::accepted, this, &UserNameButton::onRemoveUserNames);

    name_menu_->addSeparator();

    auto act = new QAction("Add User Name", this);
    connect(act, &QAction::triggered, this, &UserNameButton::showAddUserName);
    name_menu_->addAction(act);
    act->setProperty("NAME_LABEL_NOT_NAME", true);

    act = new QAction("Remove User Name(s)", this);
    connect(act, &QAction::triggered, this, &UserNameButton::showRemoveUserNames);
    act->setProperty("NAME_LABEL_NOT_NAME", true);

    name_menu_->addAction(act);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    add_user_ = new TextBoxDialog(TextBoxDialog::MODE_USER_NAME, this);
    connect(add_user_, &TextBoxDialog::accepted, this, &UserNameButton::addName);

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

void UserNameButton::paintEvent(QPaintEvent *) {
    QStylePainter p(this);
    QStyleOptionButton opt;
    initStyleOption(&opt);
    opt.features &= (~ QStyleOptionButton::HasMenu);
    p.drawControl(QStyle::CE_PushButton, opt);
}

void UserNameButton::addName() {
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
