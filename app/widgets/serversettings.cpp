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

#include "serversettings.h"
#include "ui_serversettings.h"

#include <QDebug>
#include <QFileDialog>

ServerSettingsDialog::ServerSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerSettingsDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::SplashScreen);
    setModal(true);

    connect(ui->loaderButton, SIGNAL(clicked()), SLOT(openLoaderDialog()));
    connect(ui->updateButton, SIGNAL(clicked()), SLOT(openUpdaterDialog()));
}

ServerSettingsDialog::~ServerSettingsDialog()
{
    delete ui;
}

QString ServerSettingsDialog::getLoader() const {
    return ui->loader->text();
}

QString ServerSettingsDialog::getPassword() const {
    return ui->password->text();
}

QString ServerSettingsDialog::getDMPassword() const {
    return ui->dm_password->text();
}

QString ServerSettingsDialog::getUpdater() const {
    return ui->updater->text();
}

void ServerSettingsDialog::setPassword(const QString& pass) {
    return ui->password->setText(pass);
}

void ServerSettingsDialog::setDMPassword(const QString& pass) {
    return ui->dm_password->setText(pass);
}

void ServerSettingsDialog::setLoader(const QString &l) {
    return ui->loader->setText(l);
}

void ServerSettingsDialog::setUpdater(const QString &l) {
    return ui->updater->setText(l);
}

void ServerSettingsDialog::openLoaderDialog() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Set Custom Loader", ".", "Executables (*.exe *.bat)");
    ui->loader->setText(fileName);
}

void ServerSettingsDialog::openUpdaterDialog() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Set Custom Updater", ".", "Executables (*.exe *.bat)");
    ui->updater->setText(fileName);
}

void ServerSettingsDialog::setCancelFocus() {
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setFocus();
}
