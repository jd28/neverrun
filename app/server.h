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

#ifndef SERVER_H
#define SERVER_H

#include <QString>
#include <QStringList>
#include <stdint.h>

struct Server {
    QString module_name;
    QString server_name;
    QString address;
    QString mod_description;
    QString serv_description;
    uint16_t port;
    bool ilr;
    bool elc;
    int cur_players;
    int max_players;
    int min_level;
    int max_level;
    int64_t ping;
    bool password;
    bool local_vault;
    int pvp;
    time_t heartbeat;
    bool online;
    bool one_party;
    QString homepage;
    QString nrl;
    int gametype;

    QStringList toStringList() const {
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
};

#endif // SERVER_H
