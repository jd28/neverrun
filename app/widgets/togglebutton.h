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

#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include <QWidget>

class QPushButton;

class ToggleButton : public QWidget
{
    Q_OBJECT

    QPushButton *left_;
    QPushButton *right_;
    bool is_left_;
public:
    ToggleButton(QString left, QString right, QWidget *parent = 0);

    enum Button {
        Left,
        Right
    };
signals:
    void ButtonChanged(ToggleButton::Button);

private slots:
    void OnLeftClicked(bool checked);
    void OnRightClicked(bool checked);

};

#endif // TOGGLEBUTTON_H
