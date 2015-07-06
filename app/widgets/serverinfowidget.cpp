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

#include "serverinfowidget.h"
#include "ui_serverinfowidget.h"

ServerInfoWidget::ServerInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerInfoWidget)
{
    ui->setupUi(this);
    ui->webView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->widget->layout()->setContentsMargins(2, 0, 10, 0);
    layout()->setContentsMargins(0, 0, 0, 0);
    connect(ui->closeButton, &QPushButton::clicked, this, &ServerInfoWidget::onCloseInfo);
}

ServerInfoWidget::~ServerInfoWidget()
{
    delete ui;
}

QWebView *ServerInfoWidget::webview() const {
    return ui->webView;
}

void ServerInfoWidget::setServerLabel(const QString &label)
{
    ui->serverLabel->setText(label);
}

void ServerInfoWidget::onCloseInfo() {
    emit closeInfo();
}
