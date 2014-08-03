#include <QDebug>

#include "util.h"
#include "server.h"

void Server::addPing(int64_t ping) {

    ping_history[current_ping++] = ping;
    if (current_ping >= MAX_PING_HISTORY) {
        current_ping = 0;
    }
}

int64_t Server::getPing() const {
    if(last_contact == 0 || last_query == 0) { return -1; }
    if((getTickCount() - last_contact) > 30000) { return -1; }

    int64_t sum = 0;
    for(int i = 0; i < MAX_PING_HISTORY; ++i) {
        if (ping_history[i] == -1) {
            return sum / (i + 1);
        }
        sum += ping_history[i];
    }
    return sum / MAX_PING_HISTORY;
}

bool Server::isOffline() const {
    return module_name == "Offline" || (last_query != 0 && getTickCount() - last_contact > 60000);
}

QStringList Server::toStringList() const {
    QStringList res;

    // Server Name
    res << server_name
        << module_name
        << (address + ":" + QString::number(port))
        << (QString::number(cur_players) + "/" + QString::number(max_players))
        << (QString::number(min_level) + "-" + QString::number(max_level));

    switch(pvp) {
    default: res << "None"; break;
    case 1: res << "Party"; break;
    case 2: res << "Full"; break;
    }

    res << (ilr ? "yes" : "no")
        << (elc ? "yes" : "no")
        << (local_vault ? "yes" : "no")
        << (one_party ? "yes" : "no")
        << mod_description
        << serv_description;

    return std::move(res);
}
