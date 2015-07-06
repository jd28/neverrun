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
#include "server.h"
#include "widgets/togglebutton.h"
#include "models/servertablemodel.h"

class QGridLayout;
class QNetworkDiskCache;
class QPushButton;
class QStackedWidget;
class QTableView;
class QTreeWidgetItem;
class QVBoxLayout;
class QWebView;

class DirectConnectDialog;
class FvUpdater;
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
    FvUpdater            *updater_;
    HtmlPreviewGenerator *generator_;
    ListSelectionDialog  *list_selection_dlg_;
    ModuleCategoryWidget *module_category_;
    ModuleTableWidget    *modules_table_widget_;
    QGridLayout          *main_grid_layout_;
    QMenu                *settings_button_menu_;
    QNetworkDiskCache    *diskCache;
    QStackedWidget       *cat_stack_;
    QStackedWidget       *list_stack_;
    QSettings            *settings_;
    ServerCategoryWidget *server_category_;
    ServerInfoWidget     *server_info_widget_;
    ServerTableWidget    *server_table_widget_;
    ServerWidget         *servers_;
    TextBoxDialog        *add_category_dlg_;
    ToggleButton         *servers_label_;
    UserNameButton       *name_label_;

    // Functions
    QString getDefaultNWNExe();
    QWidget *CreateCategoryWidget();
    QTableView *createModuleTable();
    QPushButton * createSettingsButton();
    void openProcess(const QString &exe, const QString &args, const QString &dir) const;
    void PlayModule(QString module, bool dm);
    void setupHtmlPreview();
    void setupUi();
    void ToolsetModule(QString module);
    void launchNWN(bool dm);
    QVariant getSelectedServerInfo(ServerTableModel::UserRoles role);

private slots:
    void about();
    void checkForUpdates();
    void dm();
    void htmlResultReady(const QString &html);
    void play();
    void launchToolset();
    void onCategoryAdded();
    void onListSelectionAccepted();
    void openSettings();
    void runUpdater();
    void switchStack();
    void requestDirectConnect();

public slots:
    void addCategory();
    void addServer(QString addr);
    void changeStack(ToggleButton::Button button);
    void loadModules(int room);
    void loadServers(int room);
    void onAddUserName(QString name);
    void onChangeUserName(QString name);
    void onRequestAddToDialog();
    void onRequestRemoveFromDialog();
    void openURL(const QUrl& url);
    void playServer(QString address, QString password, bool dm);
    void RunNWN(QString address, bool dm);
    void setServerAddressFilter(const QStringList &ips, const QString& cat);
    void setModuleFilter(const QStringList &mods, const QString &cat);
    void onRequestServerInfo(ServerInfoTypes type);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool AddServers();
    void closeEvent(QCloseEvent *event);
    Options* options() { return options_; }
    void readSettings();
    void setUpdater(FvUpdater *updater);

};

#endif // MAINWINDOW_H
