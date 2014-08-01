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

#include "mainwindow.h"

#ifdef _WIN32
#pragma comment(lib, "Shell32.lib")
#endif

#include <QApplication>
#include <QByteArray>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSplitter>
#include <QPushButton>
#include <QComboBox>
#include <QMenuBar>
#include <QTableWidget>
#include <QLabel>
#include <QUrl>
#include <QWebView>
#include <QNetworkDiskCache>
#include <QDesktopServices>
#include <QToolButton>
#include <QMenu>
#include <QProcess>
#include <qpa/qplatformnativeinterface.h>

#include "widgets/moduletablewidget.h"
#include "widgets/servertablewidget.h"
#include "widgets/servercategorywidget.h"
#include "widgets/modulecategorywidget.h"
#include "htmlpreviewgenerator.h"
#include "widgets/togglebutton.h"
#include "widgets/listselectiondialog.h"
#include "widgets/usernamebutton.h"
#include "widgets/directconnectdialog.h"

#include "models/moduletablemodel.h"
#include "widgets/setdmpassworddialog.h"
#include "util.h"
#include "widgets/serverinfowidget.h"

void MainWindow::setupUi() {
    setupHtmlPreview();
}

void MainWindow::setupHtmlPreview() {
    server_info_widget_->webview()->page()->settings()->setUserStyleSheetUrl(QUrl("qrc:/web/styles/clearness-dark.css"));
    server_info_widget_->webview()->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect( server_info_widget_->webview()->page(), SIGNAL(linkClicked(const QUrl&)),this, SLOT(openURL(const QUrl&)));

    // add our objects everytime JavaScript environment is cleared
    //connect(server_info_widget_->webview()->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
    //        this, SLOT(addJavaScriptObject()));

    // restore scrollbar position after content size changed
    //connect(server_info_widget_->webview()->page()->mainFrame(), SIGNAL(contentsSizeChanged(QSize)),
    //        this, SLOT(htmlContentSizeChanged()));

    // load HTML template for live preview from resources
    QFile f(":/web/template.html");
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString htmlTemplate = f.readAll();
        //qDebug() << "Template loaded.";
        generator_->setHtmlTemplate(htmlTemplate);
    }

    // start background HTML preview generator
    connect(generator_, SIGNAL(htmlResultReady(QString)),
            this, SLOT(htmlResultReady(QString)));
    generator_->start();
}

MainWindow::MainWindow(QWidget *parent)
    : options_(new Options(this))
    , diskCache(new QNetworkDiskCache(this))
    , generator_(new HtmlPreviewGenerator(options_, this))
    , server_info_widget_(new ServerInfoWidget(this))
    , QMainWindow(parent)
{
    options_->readSettings();

    direct_connect_dlg_ = new DirectConnectDialog(options_, this);
    name_label_ = new UserNameButton(options_, options_->getCurrentUserName(), options_->getUserNames(), this);

    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPixelSize(30);
    name_label_->setFont(font);

    server_category_ = new ServerCategoryWidget(options_, this);
    server_category_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    server_category_->setContentsMargins(0, 0, 0,0);

    main_grid_layout_ = new QGridLayout();
    main_grid_layout_->setSpacing(5);

    servers_label_ = new ToggleButton("Servers", "Modules", this);
    connect(servers_label_, SIGNAL(ButtonChanged(ToggleButton::Button)),
            SLOT(changeStack(ToggleButton::Button)));

    main_grid_layout_->addWidget(servers_label_, 0, 0);
    server_table_widget_ = new ServerTableWidget(options_, this);
    cat_stack_ = new QStackedWidget(this);
    cat_stack_->addWidget(server_category_);

    module_category_ = new ModuleCategoryWidget(options_, this);
    module_category_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    module_category_->setContentsMargins(0, 0, 0,0);

    cat_stack_->addWidget(module_category_);
    main_grid_layout_->addWidget(cat_stack_, 1, 0);

    main_grid_layout_->addWidget(createAddCategoryWidget(), 2, 0);

    list_stack_ = new QStackedWidget(this);
    list_stack_->addWidget(server_table_widget_);
    modules_table_widget_ = new ModuleTableWidget(options_, this);
    list_stack_->addWidget(modules_table_widget_);
    list_stack_->addWidget(server_info_widget_);
    main_grid_layout_->addWidget(list_stack_, 1, 1);

    main_grid_layout_->addWidget(CreateInfoButtonBar(), 2, 1);

    main_grid_layout_->setColumnStretch(0, 1);
    main_grid_layout_->setColumnStretch(1,5);

    QWidget *anotherw = new QWidget(this);
    QHBoxLayout *another = new QHBoxLayout();
    QPushButton *settings_button = new QPushButton(this);
    QIcon ico(":/qss_icons/rc/settings.png");
    settings_button->setIcon(ico);
    settings_button->setMinimumSize(QSize(35, 35));
    settings_button->setIconSize(QSize(25, 25));
    settings_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    another->addWidget(name_label_, Qt::AlignRight);
    another->addWidget(settings_button, Qt::AlignRight);
    anotherw->setLayout(another);
    another->setStretch(0, 1);
    another->setStretch(1, 1);
    anotherw->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    anotherw->setLayout(another);
    main_grid_layout_->addWidget(anotherw, 0, 1, Qt::AlignRight);

    QWidget *main_grid = new QWidget(this);
    main_grid->setLayout(main_grid_layout_);

    main_grid->setProperty("central", true);
    //w->setStyleSheet("background-image: url(:/nwn/nwn.png)");

    list_selection_dlg_ = new ListSelectionDialog(this);
    connect(list_selection_dlg_, SIGNAL(accepted()), SLOT(onListSelectionAccepted()));

    setCentralWidget(main_grid);

    connect(server_category_, SIGNAL(LoadAllServers(int)),
            SLOT(LoadServers(int)));

    connect(module_category_, SIGNAL(LoadModules(int)),
            SLOT(LoadModules(int)));

    connect(server_category_, SIGNAL(UpdateFilter(const QStringList&)),
            SLOT(SetServerAddressFilter(const QStringList&)));

    connect(module_category_, SIGNAL(UpdateFilter(const QStringList&)),
            SLOT(setModuleFilter(const QStringList&)));

    connect(server_table_widget_->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(HandleServerSelectionChange(QModelIndex,QModelIndex)));

    connect(server_table_widget_, SIGNAL(serverSettingsChanged()), SLOT(onServerSettingsChanged()));

    connect(modules_table_widget_->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(HandleModuleSelectionChange(QModelIndex,QModelIndex)));

    connect(website_button_, SIGNAL(clicked()), SLOT(HandleOpenWebsite()));

    connect(server_table_widget_, SIGNAL(RunNWN(QString,bool)),
            SLOT(RunNWN(QString,bool)));

    connect(server_table_widget_, SIGNAL(PasswordChanged(QString,QString,bool)),
            SLOT(OnPasswordChanged(QString,QString,bool)));

    connect(direct_connect_dlg_, SIGNAL(addServer(QString)),
            SLOT(addServer(QString)));

    connect(name_label_, SIGNAL(addUserName(QString)),
            SLOT(onAddUserName(QString)));
    connect(name_label_, SIGNAL(userNameChanged(QString)),
            SLOT(onChangeUserName(QString)));

    connect(server_table_widget_, SIGNAL(requestAddTo()), SLOT(onRequestAddToDialog()));
    connect(server_table_widget_, SIGNAL(requestRemoveFrom()), SLOT(onRequestRemoveFromDialog()));
    connect(modules_table_widget_, SIGNAL(requestAddTo()), SLOT(onRequestAddToDialog()));
    connect(modules_table_widget_, SIGNAL(requestRemoveFrom()), SLOT(onRequestRemoveFromDialog()));

    connect(server_info_widget_, SIGNAL(closeInfo()), SLOT(switchStack()));

    connect(direct_connect_dlg_, SIGNAL(requestPlayServer(QString,QString,bool)),
            SLOT(playServer(QString,QString,bool)));

    setupUi();

    QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    diskCache->setCacheDirectory(cacheDir);

}

void MainWindow::changeStack(ToggleButton::Button button) {
    switch(button) {
    default: return;
    case ToggleButton::Left:
        cat_stack_->setCurrentIndex(STACK_INDEX_SERVER);
        list_stack_->setCurrentIndex(STACK_INDEX_SERVER);
        website_button_->setText("Visit Website");
        info_button_->setEnabled(true);
        dm_button_->setEnabled(true);
        website_button_->setEnabled(current_website_.size() > 0);
        update_button_->setEnabled(current_server_updater_.size() > 0);
        break;
    case ToggleButton::Right:
        cat_stack_->setCurrentIndex(STACK_INDEX_MODULE);
        list_stack_->setCurrentIndex(STACK_INDEX_MODULE);
        website_button_->setText("Toolset");
        website_button_->setEnabled(true);
        info_button_->setEnabled(false);
        dm_button_->setEnabled(false);
        update_button_->setEnabled(false);
        break;
    }
}

bool MainWindow::AddServers() {
    server_category_->SelectAll();
    module_category_->SelectAll();
    generator_->markdownTextChanged(
                ""
                );

    return true;
}

void MainWindow::readSettings() {
    QSettings settings(QDir::homePath() + "/.neverrun/neverrun.ini", QSettings::IniFormat);
    restoreGeometry(settings.value("geometry", saveGeometry()).toByteArray());
    restoreState(settings.value("windowState", saveState()).toByteArray());
}

void MainWindow::htmlResultReady(const QString &html) {
    //qDebug() << html;
    server_info_widget_->webview()->page()->networkAccessManager()->setCache(diskCache);

    // remember scrollbar position
    //scrollBarPos = ui->plainTextEdit->verticalScrollBar()->value();

    QString fileName;
    // show html preview
    QUrl baseUrl;
    if (fileName.isEmpty()) {
        baseUrl = QUrl::fromLocalFile(QApplication::applicationDirPath());
    } else {
        baseUrl = QUrl::fromLocalFile(QFileInfo(fileName).absolutePath() + "/");
    }

    server_info_widget_->webview()->setHtml(html, baseUrl);

}

QWidget *MainWindow::CreateInfoButtonBar() {
    QWidget *bw = new QWidget(this);
    QHBoxLayout *bl = new QHBoxLayout();

    direct_connect_ = new QPushButton("Direct Connect", this);
    connect(direct_connect_, SIGNAL(clicked()), SLOT(requestDirectConnect()));

    play_button_ = new QPushButton("Play", this);
    connect(play_button_, SIGNAL(clicked()), SLOT(play()));
    dm_button_ = new QPushButton("DM", this);
    connect(dm_button_, SIGNAL(clicked()), SLOT(dm()));

    info_button_ = new QPushButton("Info", this);
    connect(info_button_, SIGNAL(clicked()), this, SLOT(switchStack()));

    website_button_ = new QPushButton("Visit Website", this);
    website_button_->setEnabled(false);
    website_button_->setMinimumHeight(25);

    bl->addWidget(play_button_);
    bl->addWidget(dm_button_);
    bl->addWidget(direct_connect_);
    bl->addWidget(website_button_);
    bl->addWidget(info_button_);

    update_button_ = new QPushButton("Update", this);
    update_button_->setEnabled(false);
    bl->addWidget(update_button_);
    connect(update_button_, SIGNAL(clicked()), SLOT(runUpdater()));

    bw->setLayout(bl);
    bw->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    return bw;
}

QWidget *MainWindow::createAddCategoryWidget() {
    add_category_dlg_ = new TextBoxDialog(TextBoxDialog::MODE_ADD_CATEGORY, this);
    connect(add_category_dlg_, SIGNAL(accepted()), SLOT(onCategoryAdded()));

    auto w = new QWidget(this);
    auto l = new QHBoxLayout();
    auto add_cat_button = new QPushButton("Add Category", this);
    connect(add_cat_button, SIGNAL(clicked()), SLOT(addCategory()));

    l->addWidget(add_cat_button);
    w->setLayout(l);

    return w;
}

void MainWindow::addCategory() {
    add_category_dlg_->setText("");
    add_category_dlg_->show();
}

void MainWindow::onCategoryAdded() {
    auto s = add_category_dlg_->getText();
    int cur = cat_stack_->currentIndex();

    qDebug() << s << " " << cur;

    switch(cur) {
    default: return;
    case STACK_INDEX_SERVER:
        if ( !isUsableServerCategory(s) || options_->getServerCategories().contains(s) ) {
            return;
        }
        server_category_->addCategory(s);
        break;
    case STACK_INDEX_MODULE:
        if ( !isUsableModuleCategory(s) || options_->getModuleCategories().contains(s) ) {
            return;
        }
        module_category_->addCategory(s);
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings(QDir::homePath() + "/.neverrun/neverrun.ini", QSettings::IniFormat);
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow(){
    options_->writeSettings();
    // stop background HTML preview generator
    generator_->markdownTextChanged(QString());
    generator_->wait();
    delete generator_;

}

void MainWindow::switchStack() {
    int cur = list_stack_->currentIndex();
    if ( cur == STACK_INDEX_INFO ) {
        list_stack_->setCurrentIndex(cat_stack_->currentIndex());
        info_button_->setEnabled(true);
    }
    else {
        int idx = cat_stack_->currentIndex();
        if (idx == STACK_INDEX_SERVER) {
            QModelIndexList idxs = server_table_widget_->selectionModel()->selectedRows(ServerTableModel::COLUMN_SERVER_NAME);
            if (idxs.size() == 0) { return; }
            QStringList data = server_table_widget_->model()->data(idxs[0], Qt::UserRole + 4).toStringList();
            QFile f(":/web/basic.md");
            f.open(QFile::ReadOnly);
            QString md(f.readAll());
            server_info_widget_->setServerLabel(data[0] + " - " + data[1]);
            md = md.arg(data[2], data[3], data[4], data[5], data[6], data[7], data[8])
                   .arg(data[9], data[10], data[11]);
            generator_->markdownTextChanged(md);
            list_stack_->setCurrentIndex(STACK_INDEX_INFO);
            info_button_->setEnabled(false);
        }
    }
}

void MainWindow::requestDirectConnect() {
    direct_connect_dlg_->ensureFocus();
    direct_connect_dlg_->show();
}

void MainWindow::HandleServerSelectionChange(QModelIndex current, QModelIndex previous) {
    Q_UNUSED(previous);
    if(!current.isValid()) { return; }
    QModelIndex index = server_table_widget_->model()->index(current.row(), ServerTableModel::COLUMN_SERVER_NAME);
    QString address = server_table_widget_->model()->data(index, Qt::UserRole + 3).toString();

    current_website_ = server_table_widget_->model()->data(index, Qt::UserRole + 2).toString();
    website_button_->setEnabled(current_website_.size() > 0);
    current_server_loader_ = options_->getServerLoader(address);
    current_server_updater_ = options_->getServerUpdater(address);
    update_button_->setEnabled(current_server_updater_.size() > 0);
    info_button_->setEnabled(true);
}

void MainWindow::HandleModuleSelectionChange(QModelIndex current, QModelIndex previous) {
    Q_UNUSED(previous);
    if(!current.isValid()) { return; }
    QModelIndex index = modules_table_widget_->model()->index(current.row(), 0);
    QVariant data = modules_table_widget_->model()->data(index);

    website_button_->setEnabled(true);
    dm_button_->setEnabled(false);
    info_button_->setEnabled(false);
}

void MainWindow::openURL(const QUrl &url) {
    QDesktopServices::openUrl(url);
}

void MainWindow::HandleOpenWebsite() {
    int cur = cat_stack_->currentIndex();
    switch(cur) {
    default: return;
    case STACK_INDEX_SERVER:
        openURL(QUrl(current_website_));
        break;
    case STACK_INDEX_MODULE: {
        auto selections = modules_table_widget_->selectionModel()->selectedIndexes();
        if (selections.size() == 0) { return; }
        auto idx = modules_table_widget_->model()->index(selections[0].row(), 0);
        QString module = modules_table_widget_->model()->data(idx).toString();

        module = "\"modules/"+ module + '\"';
        ToolsetModule(module);
        break;
    }
    }
}

void MainWindow::LoadServers(int room) {
    server_table_widget_->LoadServers(room, true);
}

void MainWindow::LoadModules(int room){
    modules_table_widget_->loadModules(room);
}

void MainWindow::SetServerAddressFilter(const QStringList &ips) {
    server_table_widget_->SetServerAddressFilter(ips);
    server_table_widget_->selectionModel()->clear();
}

void MainWindow::setModuleFilter(const QStringList &mods) {
    modules_table_widget_->setModuleFilter(mods);
    modules_table_widget_->selectionModel()->clear();
}

void MainWindow::ToolsetModule(QString module) {
    QString exe;
#if _WIN32
    exe = QDir::cleanPath(options_->m_NWN_path + "/NWNCdsafafX_Loader.exe");

    if (!QFile(exe).exists())
        exe = QDir::cleanPath(options_->m_NWN_path + "/nwtoolset.exe");
#endif

    QString ps = QFileInfo(exe).canonicalFilePath();
    QString dir = QFileInfo(exe).canonicalPath();
    openProcess(ps, module, dir);

}

void MainWindow::openProcess(const QString& exe, const QString& args, const QString& dir) const {
#if _WIN32
    HWND hwnd  = (HWND)QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("handle"),
                                                                                           this->windowHandle());

    int result = (int)ShellExecute(hwnd,
                                   NULL,
                                   reinterpret_cast<const WCHAR*>(exe.utf16()),
                                   reinterpret_cast<const WCHAR*>(args.utf16()),
                                   reinterpret_cast<const WCHAR*>(dir.utf16()),
                                   SW_SHOWNORMAL);
#else
    QProcess *nwn = new QProcess(this);
    nwn->setWorkingDirectory(dir);
    nwn->startDetached(ps, args);
#endif

}

void MainWindow::PlayModule(QString module, bool dm) {
    qDebug() << "Attempting to run NWN";

    QStringList arguments;
    if (dm) { return;
    }

    arguments << "+LoadNewModule"
              << module;

    QString exe;
#if _WIN32
    exe = QDir::cleanPath(options_->m_NWN_path + "/NWNCX_Loader.exe");

    if (!QFile(exe).exists())
        exe = QDir::cleanPath(options_->m_NWN_path + "/nwmain.exe");
#endif

    QString ps = QFileInfo(exe).canonicalFilePath();
    QString dir = QFileInfo(exe).canonicalPath();

    openProcess(ps, arguments.join(" "), dir);
}


void MainWindow::playServer(QString address, QString password, bool dm) {
    qDebug() << "Attempting to run NWN";
    options_->addServerToCategory("History", address);

    QStringList arguments;
    if (dm) {
        arguments << "-dmc";
    }

    if (password.size() > 0){
        arguments << "+password"
                  << password;
    }

    arguments << "+connect"
              << address;

    QString exe = current_server_loader_;
#if _WIN32
    if(exe.size() == 0 || !QFile(exe).exists())
        exe = QDir::cleanPath(options_->m_NWN_path + "/NWNCX_Loader.exe");

    if (!QFile(exe).exists())
        exe = QDir::cleanPath(options_->m_NWN_path + "/nwmain.exe");
#endif
    QString ps = QFileInfo(exe).canonicalFilePath();
    QString dir = QFileInfo(exe).canonicalPath();

    openProcess(ps, arguments.join(" "), dir);
}

void MainWindow::RunNWN(QString address, bool dm) {
    playServer(address, options_->getPassword(address, dm), dm);
}

void MainWindow::OnPasswordChanged(QString address, QString password, bool is_dm) {
    options_->setPassword(address, password, is_dm);
}

void MainWindow::onAddUserName(QString name) {
    options_->addUserName(name);
}

void MainWindow::onChangeUserName(QString name) {
    options_->setCurrentUserName(name);
}

void MainWindow::onListSelectionAccepted() {
    int cur = cat_stack_->currentIndex();
    switch(list_selection_dlg_->getMode()) {
    default: return;
    case ListSelectionDialog::MODE_ADD_TO:
        switch(cur) {
        default: return;
        case STACK_INDEX_SERVER: {
            auto sel = server_table_widget_->selectionModel()->selectedRows(ServerTableModel::COLUMN_SERVER_NAME);
            if (sel.size() == 0) { return; }
            auto add = server_table_widget_->model()->data(sel[0], Qt::UserRole + 3).toString();
            foreach(const QString& i, list_selection_dlg_->getSelectedItems()) {
                options_->addServerToCategory(i, add);
            }

            break;
        }
        case STACK_INDEX_MODULE:
            auto sel = modules_table_widget_->selectionModel()->selectedRows(0);
            if (sel.size() == 0) { return; }
            auto add = modules_table_widget_->model()->data(sel[0]).toString();
            foreach(const QString& i, list_selection_dlg_->getSelectedItems()) {
                options_->addModuleToCategory(i, add);
            }

            break;
        }
        break;
    case ListSelectionDialog::MODE_REMOVE_FROM:
        switch(cur) {
        default: return;
        case STACK_INDEX_SERVER: {
            auto sel = server_table_widget_->selectionModel()->selectedRows(ServerTableModel::COLUMN_SERVER_NAME);
            if (sel.size() == 0) { return; }
            auto add = server_table_widget_->model()->data(sel[0], Qt::UserRole + 3).toString();
            foreach(const QString& i, list_selection_dlg_->getSelectedItems()) {
                options_->removeServerFromCategory(i, add);
                auto sel = server_category_->selectionModel()->selectedRows();
                if (i == sel[0].data()) {
                    SetServerAddressFilter(options_->getCategoryIPs(i));
                }
            }

            break;
        }
        case STACK_INDEX_MODULE: {
            auto sel = modules_table_widget_->selectionModel()->selectedRows(0);
            if (sel.size() == 0) { return; }
            auto add = modules_table_widget_->model()->data(sel[0]).toString();
            foreach(const QString& i, list_selection_dlg_->getSelectedItems()) {
                options_->removeModuleFromCategory(i, add);
                auto sel = module_category_->selectionModel()->selectedRows();
                if (i == sel[0].data()) {
                    setModuleFilter(options_->getCategoryModules(i));
                }
            }
        }
        break;
        case ListSelectionDialog::MODE_REMOVE_CATEGORY: break;
        case ListSelectionDialog::MODE_REMOVE_USERNAME: break;
        }
    }
}

void MainWindow::runUpdater() {
    if( current_server_updater_.size() == 0 ) { return; }
    QString dir = QFileInfo(current_server_updater_).canonicalPath();
    openProcess(current_server_updater_, "", dir);
}

void MainWindow::onRequestAddToDialog() {
    int cur = cat_stack_->currentIndex();
    QStringList ls;
    switch(cur) {
    case STACK_INDEX_SERVER:
        ls = options_->getServerCategories();
        break;
    case STACK_INDEX_MODULE:
        ls = options_->getModuleCategories();
        break;
    }

    ls.removeAll("History");
    list_selection_dlg_->setLabel("Add to...");
    list_selection_dlg_->setMode(ListSelectionDialog::MODE_ADD_TO);
    list_selection_dlg_->setListItems(ls);
    list_selection_dlg_->show();

}

void MainWindow::onRequestRemoveFromDialog() {
    int cur = cat_stack_->currentIndex();
    QStringList ls;
    switch(cur) {
    default: return;
    case STACK_INDEX_SERVER: {
        auto selections = server_table_widget_->selectionModel()->selectedIndexes();
        if (selections.size() == 0) { return; }
        auto idx = server_table_widget_->model()->index(selections[0].row(), ServerTableModel::COLUMN_SERVER_NAME);
        QString address = server_table_widget_->model()->data(idx, Qt::UserRole + 3).toString();

        ls = options_->getCategoriesFromServer(address);
        break;
    }
    case STACK_INDEX_MODULE:  {
        auto selections = modules_table_widget_->selectionModel()->selectedIndexes();
        if (selections.size() == 0) { return; }
        auto idx = modules_table_widget_->model()->index(selections[0].row(), 0);
        QString mod = modules_table_widget_->model()->data(idx).toString();

        ls = options_->getCategoriesFromModule(mod);
        break;
    }
    }

    if (ls.size() == 0) { return; }
    list_selection_dlg_->setLabel("Remove From...");
    list_selection_dlg_->setMode(ListSelectionDialog::MODE_REMOVE_FROM);
    list_selection_dlg_->setListItems(ls);
    list_selection_dlg_->show();
}

void MainWindow::onServerSettingsChanged() {
    auto selections = server_table_widget_->selectionModel()->selectedIndexes();
    if (selections.size() == 0) { return; }
    auto idx = server_table_widget_->model()->index(selections[0].row(), ServerTableModel::COLUMN_SERVER_NAME);
    QString address = server_table_widget_->model()->data(idx, Qt::UserRole + 3).toString();

    current_server_loader_ = options_->getServerLoader(address);
    current_server_updater_ = options_->getServerUpdater(address);

    if (cat_stack_->currentIndex() == STACK_INDEX_SERVER) {
        update_button_->setEnabled(current_server_updater_.size() > 0);
    }

}

void MainWindow::play() {
    if (cat_stack_->currentIndex() == STACK_INDEX_SERVER) {
        auto selections = server_table_widget_->selectionModel()->selectedIndexes();
        if (selections.size() == 0) { return; }
        auto idx = server_table_widget_->model()->index(selections[0].row(), ServerTableModel::COLUMN_SERVER_NAME);
        QString address = server_table_widget_->model()->data(idx, Qt::UserRole + 3).toString();
        RunNWN(address, false);
    }
    else {

        auto selections = modules_table_widget_->selectionModel()->selectedIndexes();
        if (selections.size() == 0) { return; }
        auto idx = modules_table_widget_->model()->index(selections[0].row(), 0);
        QString module = modules_table_widget_->model()->data(idx).toString();

        module = '\"'+ module.mid(0, module.size() - 4) + '\"';
        PlayModule(module, false);
    }
}

void MainWindow::dm() {
    if (cat_stack_->currentIndex() != STACK_INDEX_SERVER) { return; }
    auto selections = server_table_widget_->selectionModel()->selectedIndexes();
    if (selections.size() == 0) { return; }
    auto idx = server_table_widget_->model()->index(selections[0].row(), ServerTableModel::COLUMN_SERVER_NAME);
    QString address = server_table_widget_->model()->data(idx, Qt::UserRole + 3).toString();
    RunNWN(address, true);
}

QTableView *MainWindow::createModuleTable() {
    QTableView *tbl = new QTableView(this);
    tbl->setSelectionBehavior(QAbstractItemView::SelectRows);
    tbl->setSelectionMode(QAbstractItemView::SingleSelection);

    auto model = new ModuleTableModel(options_, this);
    tbl->setModel(model);
    tbl->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tbl->setSortingEnabled(true);
    tbl->verticalHeader()->setVisible(false);
    return tbl;
}

void MainWindow::addServer(QString addr) {
    server_table_widget_->addServer(addr);
    auto it = server_category_->selectionModel()->selectedIndexes();
    if (it.size() == 0) { return; }

    if (it[0].data().toString() == "History") {
        SetServerAddressFilter(options_->getCategoryIPs("History"));
    }
}
