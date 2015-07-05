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

#include <QAction>
#include <QDebug>
#include <QErrorMessage>
#include <QHeaderView>
#include <QHostAddress>
#include <QItemDelegate>
#include <QMenu>
#include <QPainter>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QtConcurrent/QtConcurrentRun>
#include <QtEndian>
#include <QTimer>
#include <QUdpSocket>

#include "servertablewidget.h"
#include "setdmpassworddialog.h"
#include "../delegates/imagedelegate.h"
#include "../server.h"
#include "serversettings.h"
#include "../options.h"
#include "../util.h"

ServerTableWidget::ServerTableWidget(Options *options, QWidget *parent)
    : QTableView(parent)
    , current_room_(-1)
    , requested_room_(-1)
    , server_settings_dlg_(new ServerSettingsDialog(this))
    , options_(options)
    , error_loading_(false)
{

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &ServerTableWidget::customContextMenuRequested,
            this, &ServerTableWidget::customMenuRequested);

    setHorizontalHeader(new HeaderView(Qt::Horizontal, this));
    auto image_delegate = new ImageDelegate(this);
    setItemDelegateForColumn(ServerTableModel::COLUMN_PASSWORD, image_delegate);
    model_ = new ServerTableModel(this);
    proxy_model_ = new ServerTableProxyModel(this);
    proxy_model_->setSourceModel(model_);
    proxy_model_->setDynamicSortFilter(true);
    setModel(proxy_model_);
    horizontalHeader()->setSortIndicatorShown(true);
    horizontalHeader()->setSectionsClickable(true);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(ServerTableModel::COLUMN_PASSWORD, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(ServerTableModel::COLUMN_PLAYER_COUNT, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(ServerTableModel::COLUMN_PING, QHeaderView::ResizeToContents);

    setSortingEnabled(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    verticalHeader()->setVisible(false);

    horizontalHeader()->setSortIndicator(ServerTableModel::COLUMN_PLAYER_COUNT, Qt::DescendingOrder);

    timer_ = new QTimer(this);
    timer_->start(60000);
    connect(timer_, &QTimer::timeout, [this](){ if (canUpdate()) { getServerList(-1); }});

    connect(server_settings_dlg_, &ServerSettingsDialog::accepted, this, &ServerTableWidget::onSettingsChanged);
}

void ServerTableWidget::onRemoveFromCat() {
    QString address = getSelectedServerInfo(ServerTableModel::USER_ROLE_ADDRESS).toString();
    options_->removeServerFromCategory(current_cat_, address);
    setServerAddressFilter(options_->getCategoryIPs(current_cat_), current_cat_);
}

void ServerTableWidget::onBlacklist() {
    QString address = getSelectedServerInfo(ServerTableModel::USER_ROLE_IP).toString();
    options_->addToBlacklist(address);
    proxy_model_->setServerBlacklist(options_->getBlacklist());
}

QVariant ServerTableWidget::getSelectedServerInfo(ServerTableModel::UserRoles role) {
    auto selections = selectedIndexes();
    if (selections.size() == 0) { return QVariant(); }
    auto idx = model()->index(selections[0].row(), ServerTableModel::COLUMN_SERVER_NAME);
    return model()->data(idx, role);
}

static std::vector<Server> GetAllServers(QStringList history){
    NWNMasterServerAPIProxy *api = new NWNMasterServerAPIProxy();
    ArrayOfNWGameServer *servers;
    api->soap_endpoint = API_ENDPOINT;

    std::vector<Server> s;
    GetOnlineServerList srv_request;
    GetOnlineServerListResponse srv_response;

    srv_request.Product = (char *)soap_malloc(api, sizeof(char) * 5);  // NWN1 plus a null character.
    strcpy_s(srv_request.Product, sizeof(char) * 5,  "NWN1");

    int res = api->GetOnlineServerList(&srv_request, &srv_response);
    if(res != SOAP_OK) {
        delete api;
        return s;
    }

    servers = srv_response.GetOnlineServerListResult;

    if(servers == NULL) {
        delete api;
        return s;
    }

    s.reserve(servers->__sizeNWGameServer + history.size());
    for(int i = 0; i < servers->__sizeNWGameServer; ++i){
        QString add(servers->NWGameServer[i]->ServerAddress);
        history.removeAll(add);

        Server serv;
        serv.module_name = servers->NWGameServer[i]->ModuleName;
        serv.server_name = sanitizeName(servers->NWGameServer[i]->ServerName);
        if (serv.server_name.size() == 0)
            serv.server_name = "Unnamed Server";

        QRegExp rx("(\\:)");
        QStringList query = add.split(rx);
        Q_ASSERT(query.size() == 2);
        serv.address = query[0];
        serv.port = query[1].toInt();
        serv.ilr = *servers->NWGameServer[i]->ILREnforced;
        serv.elc = *servers->NWGameServer[i]->ELCEnforced;
        serv.cur_players = *servers->NWGameServer[i]->ActivePlayerCount;
        serv.max_players = *servers->NWGameServer[i]->MaximumPlayerCount;
        serv.min_level = *servers->NWGameServer[i]->MinimumLevel;
        serv.password = *servers->NWGameServer[i]->PrivateServer;
        serv.max_level = *servers->NWGameServer[i]->MaximumLevel;
        serv.local_vault = *servers->NWGameServer[i]->LocalVault;
        serv.pvp = *servers->NWGameServer[i]->PVPLevel;
        serv.heartbeat = *servers->NWGameServer[i]->LastHeartbeat;
        serv.online = *servers->NWGameServer[i]->Online;
        serv.one_party = *servers->NWGameServer[i]->OnePartyOnly;
        serv.gametype = *servers->NWGameServer[i]->GameType;
        QString desc(servers->NWGameServer[i]->ServerDescription);
        serv.mod_description = servers->NWGameServer[i]->ModuleDescription;
        serv.mod_description.replace(QRegExp("[\\n\\r]+"), "\n\n");

        QString url = findUrl(desc);
        if (url.size() == 0) {
            url = findUrl(serv.mod_description);
        }
        if (url.size() > 0) {
            if ( url.endsWith(".nrl", Qt::CaseInsensitive)) {
                serv.nrl = url;
            }
            else {
                serv.homepage = url;
            }
        }
        serv.serv_description = desc;
        serv.serv_description.replace(QRegExp("[\\n\\r]+"), "\n\n");
        s.push_back(serv);
    }

    for(auto it = history.begin(); it != history.end(); ++it) {
        Server serv;
        QRegExp rx("(\\:)");
        QStringList query = (*it).split(rx);
        Q_ASSERT(query.size() == 2);
        serv.address = query[0];
        serv.port = query[1].toInt();
        serv.module_name = "Offline";
        serv.server_name = *it;
        serv.online = false;
        s.push_back(serv);
    }

    delete api;

    return s;
}

void ServerTableWidget::getServerList(int room, bool force) {
    if(!options_->getUpdateBackground() && !force && !isVisible()) { return; }
    requested_room_ = room;
    QFuture<std::vector<Server>> future = QtConcurrent::run(GetAllServers, options_->getCategoryIPs("History"));
    connect(&watcher_, &ServerFutureWatcher::finished, this, &ServerTableWidget::finished);
    watcher_.setFuture(future);
}

void ServerTableWidget::loadServers(int room, bool force) {
    if (model_->rowCount(QModelIndex()) > 0) {
        ServerTableProxyModel* pm = reinterpret_cast<ServerTableProxyModel*>(model());
        pm->setCurrentRoom(room);
        model_->setCurrentRoom(room);
        return;
    }
    getServerList(-1, force);
}

void ServerTableWidget::finished(){
    setUpdatesEnabled(false);
    ServerTableProxyModel* pm = reinterpret_cast<ServerTableProxyModel*>(model());

    std::vector<Server> res(std::move(watcher_.result()));
    if ( res.size() == 0) {
        if (!error_loading_) {
            errorMessage("Error loading server list!", this);
        }
        error_loading_ = true;
        return;
    }

    error_loading_ = false;
    if(model_->rowCount(QModelIndex()) == 0) {
        model_ = new ServerTableModel(std::move(res), this);
        pm->setSourceModel(model_);
        pm->setServerBlacklist(options_->getBlacklist());
        model_->bindUpdSocket(options_->getClientPort() + 100);
        setBNXR();
        setBNDR();
        setBNERU();
        connect(model_, &ServerTableModel::dataChanged, this, &ServerTableWidget::onModelDataChanged);
        ping_timer_ = new QTimer(this);
        connect(ping_timer_, &QTimer::timeout, this, &ServerTableWidget::requestUpdates);
        ping_timer_->start(15000/model_->rowCount(QModelIndex()));

        offline_timer_ = new QTimer(this);
        connect(offline_timer_, &QTimer::timeout, this, &ServerTableWidget::sweepOfflineServers);
        offline_timer_->start(15000);

    }
    else {
        model_->UpdateSevers(std::move(res));
    }

    current_room_ = requested_room_;

    if(ServerTableModel::COLUMN_PLAYER_COUNT == horizontalHeader()->sortIndicatorSection()){
        pm->invalidate();
    }

    setUpdatesEnabled(true);
}

void ServerTableWidget::onModelDataChanged(QModelIndex,QModelIndex) {
    ServerTableProxyModel* pm = reinterpret_cast<ServerTableProxyModel*>(model());
    if(ServerTableModel::COLUMN_PLAYER_COUNT == horizontalHeader()->sortIndicatorSection()){
        pm->invalidate();
    }
    else if(ServerTableModel::COLUMN_PING == horizontalHeader()->sortIndicatorSection()){
        pm->invalidate();
    }
}

bool ServerTableWidget::canUpdate() {
    static bool locked = false;
    if(options_->getUpdateBackground()) {
        locked = false;
        return true;
    }
    if ( !isVisible() || !isActiveWindow() || parentWidget()->isMinimized()) {
        locked = true;
        return false;
    }

    if (locked) {
        model_->setLastActive(getTickCount());
    }

    locked = false;
    return true;
}

void ServerTableWidget::addServer(const QString& addr) {
    model_->addServer(addr);
}

void ServerTableWidget::onSettingsChanged() {
    QString address = getSelectedServerInfo(ServerTableModel::USER_ROLE_ADDRESS).toString();
    options_->setServerLoader(address, server_settings_dlg_->getLoader());
    options_->setServerUpdater(address, server_settings_dlg_->getUpdater());
    options_->setPassword(address, server_settings_dlg_->getDMPassword(), true);
    options_->setPassword(address, server_settings_dlg_->getPassword(), false);
}

void ServerTableWidget::customMenuRequested(QPoint pos) {
    QModelIndex index = indexAt(pos);
    if (!index.isValid()) { return; }

    QMenu menu(this);

    QAction *act = menu.addAction("Play");
    connect(act, &QAction::triggered, [this]() { emit play(); });

    act = menu.addAction("DM");
    connect(act, &QAction::triggered, [this]() { emit dm(); });

    menu.addSeparator();

    act = menu.addAction("Description");
    connect(act, &QAction::triggered, [this]() { emit serverInfoRequest(SERVER_INFO_TYPE_DESCRIPTION); });

    QMenu *community_menu = menu.addMenu("Community");

    act = community_menu->addAction("Website");
    if(getSelectedServerInfo(ServerTableModel::USER_ROLE_WEBPAGE).toString().size() > 0) {
        connect(act, &QAction::triggered, [this]() { emit serverInfoRequest(SERVER_INFO_TYPE_WEBSITE); });
    }
    else { act->setDisabled(true); }

    act = community_menu->addAction("Forum");
    if(getSelectedServerInfo(ServerTableModel::USER_ROLE_FORUM).toString().size() > 0) {
        connect(act, &QAction::triggered, [this]() { emit serverInfoRequest(SERVER_INFO_TYPE_FORUM); });
    }
    else { act->setDisabled(true); }

    act = community_menu->addAction("Web Chat");
    if(getSelectedServerInfo(ServerTableModel::USER_ROLE_CHAT).toString().size() > 0) {
        connect(act, &QAction::triggered, [this]() { emit serverInfoRequest(SERVER_INFO_TYPE_CHAT); });
    }
    else { act->setDisabled(true); }

    act = menu.addAction("Update");
    QString nrl = getSelectedServerInfo(ServerTableModel::USER_ROLE_UPDATE).toString();
    QString address = getSelectedServerInfo(ServerTableModel::USER_ROLE_ADDRESS).toString();
    if(nrl.size() > 0 || options_->getServerUpdater(address).size() > 0) {
        connect(act, &QAction::triggered, [this]() { emit update(); });
    }
    else { act->setDisabled(true); }

    menu.addSeparator();

    act = menu.addAction("Add to...");
    connect(act, &QAction::triggered, [this] () { emit requestAddTo(); });

    act = menu.addAction("Remove from...");
    connect(act, &QAction::triggered, [this]() { emit requestRemoveFrom(); });

    if(current_cat_.size() > 0) {
        act = menu.addAction("Remove from " + current_cat_);
        connect(act, &QAction::triggered, this, &ServerTableWidget::onRemoveFromCat);
    }

    menu.addSeparator();

    act = menu.addAction("Hide IP Address");
    connect(act, &QAction::triggered, this, &ServerTableWidget::onBlacklist);

    menu.addSeparator();

    act = menu.addAction("Settings");
    connect(act, &QAction::triggered, this, &ServerTableWidget::requestChangeServerSettings);

    menu.exec(viewport()->mapToGlobal(pos));
}

void ServerTableWidget::requestChangeServerSettings() {
    QString address = getSelectedServerInfo(ServerTableModel::USER_ROLE_ADDRESS).toString();
    server_settings_dlg_->setDMPassword(options_->getPassword(address, true));
    server_settings_dlg_->setPassword(options_->getPassword(address, false));
    server_settings_dlg_->setLoader(options_->getServerLoader(address));
    server_settings_dlg_->setUpdater(options_->getServerUpdater(address));
    server_settings_dlg_->setCancelFocus();
    server_settings_dlg_->show();
}

void ServerTableWidget::setServerAddressFilter(const QStringList& ips, const QString& cat) {
    current_cat_ = cat;
    model_->setServerAddressFilter(ips);
    proxy_model_->setServerAddressFilter(ips);
}

void ServerTableWidget::updateBlacklist() {
    proxy_model_->setServerBlacklist(options_->getBlacklist());
}

void ServerTableWidget::requestUpdates() {
    if (canUpdate()) { model_->requestUpdates(); }
}

void ServerTableWidget::sweepOfflineServers() {
    if (canUpdate()) { model_->sweepOfflineServers(); }
}

void ServerTableWidget::setBNXR() {
    QByteArray bnxi("BNXI");
    uint16_t p = options_->getClientPort() + 100;
    char res[2];
    qToBigEndian(p, (uchar*)&res);
    bnxi.append(res, 2);
    model_->setBNXI(bnxi);
}

void ServerTableWidget::setBNDR() {
    QByteArray bnxi("BNDS");
    uint16_t p = options_->getClientPort() + 100;
    char res[2];
    qToBigEndian(p, (uchar*)&res);
    bnxi.append(res, 2);
    model_->setBNDS(bnxi);
}

void ServerTableWidget::setBNERU() {
    QByteArray bnxi("BNES");
    uint16_t p = options_->getClientPort() + 100;
    char res[2];
    qToBigEndian(p, (uchar*)&res);
    bnxi.append(res, 2);
    model_->setBNES(bnxi);
}

