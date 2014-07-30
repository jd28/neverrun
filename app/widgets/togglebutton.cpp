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

#include "togglebutton.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>

static const char *leftstyle = (
            "QPushButton {"
            "    background-color: #201F1F;"
            "    padding-top: 5px;"
            "    padding-bottom: 5px;"
            "    padding-right: 30px;"
            "    padding-left: 30px;"
            "    border-bottom-left-radius: 3px;"
            "    border-top-left-radius: 3px;"
            "    border-bottom-right-radius: 0px;"
            "    border-top-right-radius: 0px;"
            "    border-top: 1px solid #3A3939;"
            "    border-bottom: 1px solid #3A3939;"
            "    border-right: transparent;"
            "    border-left: 1px solid #3A3939;"
            "}"
            "QPushButton:hover {"
            "background-color: #4A4949;"
            "    border-top: 1px solid #78879b;"
            "    border-bottom: 1px solid #78879b;"
//            "    border-right: transparent;"
            "    border-left: 1px solid #78879b;"
            "}"
            "QPushButton:checked {"
            "    background-color: #78879b;"
            "    border-top: 1px solid #78879b;"
            "    border-bottom: 1px solid #78879b;"
//            "    border-right: transparent;"
            "    border-left: 1px solid #78879b;"
            "}"
            );

static const char *rightstyle = (
            "QPushButton {"
            "    background-color: #201F1F;"
            "    padding-top: 5px;"
            "    padding-bottom: 5px;"
            "    padding-right: 30px;"
            "    padding-left: 30px;"
            "    border-bottom-right-radius: 3px;"
            "    border-top-right-radius: 3px;"
            "    border-bottom-left-radius: 0px;"
            "    border-top-left-radius: 0px;"
            "    border-top: 1px solid #3A3939;"
            "    border-bottom: 1px solid #3A3939;"
            "    border-left: transparent;"
            "    border-right: 1px solid #3A3939;"
            "}"
            "QPushButton:hover {"
            "background-color: #4A4949;"
            "    border-top: 1px solid #78879b;"
            "    border-bottom: 1px solid #78879b;"
//            "    border-left: transparent;"
            "    border-right: 1px solid #78879b;"
            "}"
            "QPushButton:checked {"
            "    background-color: #78879b;"
            "    border-top: 1px solid #78879b;"
            "    border-bottom: 1px solid #78879b;"
//            "    border-left: transparent;"
            "    border-right: 1px solid #78879b;"
            "}"
            );

ToggleButton::ToggleButton(QString left, QString right, QWidget *parent) :
    QWidget(parent)
{
    is_left_ = true;
    auto layout = new QHBoxLayout();
    left_ = new QPushButton(left, this);
    right_ = new QPushButton(right, this);

    left_->setCheckable(true);
    left_->setChecked(true);
    left_->setStyleSheet(leftstyle);
    right_->setStyleSheet(rightstyle);
    right_->setCheckable(true);

    layout->addWidget(left_);
    layout->addWidget(right_);
    setLayout(layout);
    layout->setSpacing(0);

    connect(left_,SIGNAL(clicked(bool)),
            this, SLOT(OnLeftClicked(bool)));
    connect(right_,SIGNAL(clicked(bool)),
            this, SLOT(OnRightClicked(bool)));

}

void ToggleButton::OnLeftClicked(bool checked) {
    if(is_left_) {
        left_->setChecked(true);
        return;
    }
    is_left_ = true;
    right_->setChecked(false);
    emit ButtonChanged(Left);
}

void ToggleButton::OnRightClicked(bool checked)
{
    if(!is_left_) {
        right_->setChecked(true);
        return;
    }
    is_left_ = false;
    left_->setChecked(false);
    emit ButtonChanged(Right);
}
