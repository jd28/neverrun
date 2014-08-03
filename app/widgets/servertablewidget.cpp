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
{

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(customMenuRequested(QPoint)));

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

    connect(timer_, SIGNAL(timeout()), SLOT(UpdateServers()));
    act_add_to_ = new QAction("Add to...", this);
    connect(act_add_to_, SIGNAL(triggered()), SLOT(onAddTo()));
    act_remove_from_ = new QAction("Remove from...", this);
    connect(act_remove_from_, SIGNAL(triggered()), SLOT(onRemoveFrom()));
    act_remove_from_cat_ = new QAction("Remove from", this);
    connect(act_remove_from_cat_, SIGNAL(triggered()), SLOT(onRemoveFromCat()));

    act_settings_ = new QAction("Settings", this);
    connect(act_settings_, SIGNAL(triggered()), SLOT(requestChangeServerSettings()));

    connect(server_settings_dlg_, SIGNAL(accepted()), SLOT(onSettingsChanged()));

    SetupDialogs();
}

void ServerTableWidget::onAddTo() {
    emit requestAddTo();
}

void ServerTableWidget::onRemoveFrom() {
    emit requestRemoveFrom();
}

void ServerTableWidget::onRemoveFromCat() {
    auto selections = selectedIndexes();
    if (selections.size() == 0) { return; }
    auto idx = model()->index(selections[0].row(), ServerTableModel::COLUMN_SERVER_NAME);
    QString address = model()->data(idx, Qt::UserRole + 3).toString();
    options_->removeServerFromCategory(current_cat_, address);
    SetServerAddressFilter(options_->getCategoryIPs(current_cat_), current_cat_);
}

void ServerTableWidget::SetupDialogs() {
    connect(server_settings_dlg_, SIGNAL(accepted()),
            SLOT(onSettingsChanged()));

}

const ServerTableModel* ServerTableWidget::getServerTableModel() const {
    return model_;
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
            serv.server_name = "Unamed Server";

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

void ServerTableWidget::GetServerList(int room, bool force) {
    if(!force && !isVisible()) { return; }
    requested_room_ = room;
    QFuture<std::vector<Server>> future = QtConcurrent::run(GetAllServers, options_->getCategoryIPs("History"));
    connect(&watcher_, SIGNAL(finished()), this, SLOT(finished()));
    watcher_.setFuture(future);
}

void ServerTableWidget::LoadServers(int room, bool force) {
    if (model_->rowCount(QModelIndex()) > 0) {
        ServerTableProxyModel* pm = reinterpret_cast<ServerTableProxyModel*>(model());
        pm->setCurrentRoom(room);
        return;
    }
    GetServerList(-1, force);
}

void ServerTableWidget::finished(){
    setUpdatesEnabled(false);
    ServerTableProxyModel* pm = reinterpret_cast<ServerTableProxyModel*>(model());

    std::vector<Server> res(std::move(watcher_.result()));
    if ( res.size() == 0) {
        QErrorMessage err;
        err.showMessage("Error loading server list!");
    }

    if(model_->rowCount(QModelIndex()) == 0) {
        model_ = new ServerTableModel(std::move(res), this);
        pm->setSourceModel(model_);
        model_->bindUpdSocket(options_->getClientPort() + 100);
        setBNXR();
        setBNDR();
        setBNERU();
        connect(model_,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                SLOT(onModelDataChanged(QModelIndex,QModelIndex)));
        ping_timer_ = new QTimer(this);
        connect(ping_timer_, SIGNAL(timeout()), SLOT(requestUpdates()));
        ping_timer_->start(50);

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

void ServerTableWidget::UpdateServers() {
    GetServerList(-1);
}

void ServerTableWidget::HandleSelectionChange(QModelIndex current, QModelIndex previous) {
    Q_UNUSED(current);
    Q_UNUSED(previous);
}

void ServerTableWidget::addServer(const QString& addr) {
    model_->addServer(addr);
}

void ServerTableWidget::onSettingsChanged() {
    auto selections = selectedIndexes();
    if (selections.size() == 0) { return; }
    auto idx = model()->index(selections[0].row(), ServerTableModel::COLUMN_SERVER_NAME);
    QString address = model()->data(idx, Qt::UserRole + 3).toString();

    options_->setServerLoader(address, server_settings_dlg_->getLoader());
    options_->setServerUpdater(address, server_settings_dlg_->getUpdater());
    options_->setPassword(address, server_settings_dlg_->getDMPassword(), true);
    options_->setPassword(address, server_settings_dlg_->getPassword(), false);

    emit serverSettingsChanged();
}

void ServerTableWidget::customMenuRequested(QPoint pos) {
    QModelIndex index = indexAt(pos);
    if (!index.isValid()) { return; }

    QMenu menu(this);

    auto act = new QAction("Play", &menu);
    connect(act, SIGNAL(triggered()), this, SLOT(play()));
    menu.addAction(act);


    act = new QAction("DM", &menu);
    connect(act, SIGNAL(triggered()), this, SLOT(dm()));
    menu.addAction(act);

    menu.addSeparator();

    menu.addAction(act_add_to_);
    menu.addAction(act_remove_from_);
    if(current_cat_.size() > 0) {
        act_remove_from_cat_->setText("Remove from " + current_cat_);
        menu.addAction(act_remove_from_cat_);
    }

    menu.addSeparator();
    menu.addAction(act_settings_);

    menu.exec(viewport()->mapToGlobal(pos));
}

void ServerTableWidget::play() {
    auto selections = selectedIndexes();
    if (selections.size() == 0) { return; }
    auto idx = model()->index(selections[0].row(), ServerTableModel::COLUMN_SERVER_NAME);
    QString address = model()->data(idx, Qt::UserRole + 3).toString();
    emit RunNWN(address, false);
}

void ServerTableWidget::dm() {
    auto selections = selectedIndexes();
    if (selections.size() == 0) { return; }
    auto idx = model()->index(selections[0].row(), ServerTableModel::COLUMN_SERVER_NAME);
    QString address = model()->data(idx, Qt::UserRole + 3).toString();
    emit RunNWN(address, true);
}

void ServerTableWidget::requestChangeServerSettings() {
    auto selections = selectedIndexes();
    if (selections.size() == 0) { return; }
    auto idx = model()->index(selections[0].row(), ServerTableModel::COLUMN_SERVER_NAME);
    QString address = model()->data(idx, Qt::UserRole + 3).toString();

    server_settings_dlg_->setDMPassword(options_->getPassword(address, true));
    server_settings_dlg_->setPassword(options_->getPassword(address, false));
    server_settings_dlg_->setLoader(options_->getServerLoader(address));
    server_settings_dlg_->setUpdater(options_->getServerUpdater(address));
    server_settings_dlg_->setCancelFocus();
    server_settings_dlg_->show();
}

void ServerTableWidget::SetServerAddressFilter(const QStringList& ips, const QString& cat) {
    current_cat_ = cat;
    proxy_model_->SetServerAddressFilter(ips);
}

void ServerTableWidget::requestUpdates() {
    if ( !isVisible() || !isActiveWindow() || parentWidget()->isMinimized())
        return;

    model_->requestUpdates();
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

