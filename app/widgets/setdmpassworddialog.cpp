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

#include "setdmpassworddialog.h"
#include "ui_setdmpassworddialog.h"

TextBoxDialog::TextBoxDialog(QWidget *parent) :
    QDialog(parent),
    mode_(MODE_DM_PASSWORD),
    ui(new Ui::SetDMPasswordDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::SplashScreen);
    setModal(true);
    setEntryMode(mode_);

}

TextBoxDialog::TextBoxDialog(Mode mode, QWidget *parent)
    : QDialog(parent)
    , mode_(mode)
    , ui(new Ui::SetDMPasswordDialog) {
    ui->setupUi(this);

    setWindowFlags(Qt::SplashScreen);
    setModal(true);
    setEntryMode(mode);

}


TextBoxDialog::~TextBoxDialog()
{
    delete ui;
}

void TextBoxDialog::setEntryMode(Mode mode) {
    mode_ = mode;
    switch(mode) {
    case MODE_USER_NAME: ui->groupBox->setTitle("Add User Name"); break;
    case MODE_DM_PASSWORD: ui->groupBox->setTitle("DM Password"); break;
    case MODE_PLAYER_PASSWORD: ui->groupBox->setTitle("Player Password"); break;
    case MODE_ADD_CATEGORY: ui->groupBox->setTitle("Add Category"); break;
    }
}

TextBoxDialog::Mode TextBoxDialog::getEntryMode() const{
    return mode_;
}

QString TextBoxDialog::getText() const {
    return ui->passwordBox->text();
}

void TextBoxDialog::setText(const QString &text) {
    ui->passwordBox->setText(text);
}
