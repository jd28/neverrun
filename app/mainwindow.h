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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QTreeWidget>
#include "options.h"

#include "widgets/togglebutton.h"

class QGridLayout;
class QNetworkDiskCache;
class QPushButton;
class QStackedWidget;
class QTableView;
class QTreeWidgetItem;
class QVBoxLayout;
class QWebView;

class DirectConnectDialog;
class HtmlPreviewGenerator;
class ListSelectionDialog;
class ModuleCategoryWidget;
class ModuleTableWidget;
class ServerCategoryWidget;
class ServerInfoWidget;
class ServerTableWidget;
class ServerWidget;
class TextBoxDialog;
class ToggleButton;
class UserNameButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

    enum StackIndex {
        STACK_INDEX_SERVER,
        STACK_INDEX_MODULE,
        STACK_INDEX_INFO
    };

    // Data Members
    Options              *options_;
    DirectConnectDialog  *direct_connect_dlg_;
    HtmlPreviewGenerator *generator_;
    ListSelectionDialog  *list_selection_dlg_;
    ModuleCategoryWidget *module_category_;
    ModuleTableWidget    *modules_table_widget_;
    QGridLayout          *main_grid_layout_;
    QNetworkDiskCache    *diskCache;
    QPushButton          *website_button_;
    QPushButton          *info_button_;
    QPushButton          *play_button_;
    QPushButton          *dm_button_;
    QPushButton          *direct_connect_;
    QPushButton          *update_button_;
    QStackedWidget       *cat_stack_;
    QStackedWidget       *list_stack_;
    QString               current_website_;
    QString               current_server_updater_;
    QString               current_server_loader_;
    QSettings            *settings_;
    ServerCategoryWidget *server_category_;
    ServerInfoWidget     *server_info_widget_;
    ServerTableWidget    *server_table_widget_;
    ServerWidget         *servers_;
    TextBoxDialog        *add_category_dlg_;
    ToggleButton         *servers_label_;
    UserNameButton       *name_label_;

    // Functions
    QWidget *createAddCategoryWidget();
    QWidget *CreateCategoryWidget();
    QWidget *CreateInfoButtonBar();
    QTableView *createModuleTable();
    QWidget *CreatePlayButtonBar();
    void openProcess(const QString &exe, const QString &args, const QString &dir) const;
    void PlayModule(QString module, bool dm);
    void setupHtmlPreview();
    void setupUi();
    void ToolsetModule(QString module);

private slots:
    void addCategory();
    void htmlResultReady(const QString &html);
    void play();
    void dm();
    void onCategoryAdded();
    void onDoubleClickServer(QModelIndex idx);
    void onListSelectionAccepted();
    void runUpdater();
    void switchStack();
    void requestDirectConnect();

public slots:
    void addServer(QString addr);
    void changeStack(ToggleButton::Button button);
    void HandleModuleSelectionChange(QModelIndex current, QModelIndex previous);
    void HandleOpenWebsite();
    void HandleServerSelectionChange(QModelIndex current, QModelIndex previous);
    void LoadModules(int room);
    void LoadServers(int room);
    void onAddUserName(QString name);
    void onChangeUserName(QString name);
    void OnPasswordChanged(QString address, QString password, bool is_dm);
    void onRequestAddToDialog();
    void onRequestRemoveFromDialog();
    void onServerSettingsChanged();
    void openURL(const QUrl& url);
    void playServer(QString address, QString password, bool dm);
    void RunNWN(QString address, bool dm);
    void SetServerAddressFilter(const QStringList &ips);
    void setModuleFilter(const QStringList &mods);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool AddServers();
    void closeEvent(QCloseEvent *event);
    Options* options() { return options_; }
    void readSettings();

};

#endif // MAINWINDOW_H
