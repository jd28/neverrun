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

#include <QDebug>
#include <QString>
#include <QStringList>
#include <stdint.h>

struct Server {
    static const int MAX_PING_HISTORY = 2;

    Server()
        : ilr(false)
        , elc(false)
        , cur_players(0)
        , max_players(0)
        , min_level(0)
        , max_level(0)
        , password(false)
        , local_vault(false)
        , pvp(0)
        , online(true)
        , one_party(false)
        , gametype(-1)
        , last_query(0)
        , last_contact(0)
        , current_ping(0)
    {
        std::fill_n(ping_history, MAX_PING_HISTORY, -1);
    }

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
    bool password;
    bool local_vault;
    int pvp;
    time_t heartbeat;
    bool online;
    bool one_party;
    QString homepage;
    QString nrl;
    int gametype;
    uint64_t last_query;
    uint64_t last_contact;
    ServerMessagesReceived messages_received;
    int64_t ping_history[MAX_PING_HISTORY];
    int current_ping;

    void addPing(int64_t ping);
    int64_t getPing() const;
    QStringList toStringList() const;


};

#endif // SERVER_H
