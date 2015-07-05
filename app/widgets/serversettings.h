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

#ifndef SERVERSETTINGS_H
#define SERVERSETTINGS_H

#include <QDialog>

namespace Ui {
class ServerSettingsDialog;
}

class ServerSettingsDialog : public QDialog
{
    Q_OBJECT

    Ui::ServerSettingsDialog *ui;

private slots:
    void openLoaderDialog();
    void openUpdaterDialog();

public:
    explicit ServerSettingsDialog(QWidget *parent = 0);
    ~ServerSettingsDialog();

    QString getLoader() const;
    QString getDMPassword() const;
    QString getPassword() const;
    QString getUpdater() const;

    void setLoader(const QString& l);
    void setDMPassword(const QString &pass);
    void setPassword(const QString &pass);
    void setUpdater(const QString& l);
    void setCancelFocus();
};

#endif // SERVERSETTINGS_H
