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

enum ServerInfoTypes {
    SERVER_INFO_TYPE_DESCRIPTION,
    SERVER_INFO_TYPE_WEBSITE,
    SERVER_INFO_TYPE_FORUM,
    SERVER_INFO_TYPE_CHAT,
    SERVER_INFO_TYPE_UPDATE,
    SERVER_INFO_TYPE_NUM
};

static const uint32_t SERVER_MESSAGES_RECIEVED_NONE = 0;
static const uint32_t SERVER_MESSAGES_RECIEVED_BNXR = 0x1;
static const uint32_t SERVER_MESSAGES_RECIEVED_BNER = 0x2;
static const uint32_t SERVER_MESSAGES_RECIEVED_BNDR = 0x4;

struct Server {
    static const int MAX_PING_HISTORY = 2;

    Server()
        : last_query(0)
        , last_contact(0)
        , current_ping(0)
        , cur_players(0)
        , max_players(0)
        , min_level(0)
        , max_level(0)
        , pvp(0)
        , gametype(-1)
        , messages_received(SERVER_MESSAGES_RECIEVED_NONE)
        , ilr(false)
        , elc(false)
        , password(false)
        , local_vault(false)
        , online(true)
        , one_party(false)
    {
        std::fill_n(ping_history, MAX_PING_HISTORY, -1);
    }

    QString module_name;
    QString server_name;
    QString address;
    QString mod_description;
    QString serv_description;
    QString homepage;
    QString forum;
    QString chat;
    QString nrl;
    uint64_t last_query;
    uint64_t last_contact;
    int64_t ping_history[MAX_PING_HISTORY];
    time_t heartbeat;
    uint16_t port;
    int cur_players;
    int max_players;
    int min_level;
    int max_level;
    int pvp;
    int gametype;
    uint32_t messages_received;
    int current_ping;
    bool ilr;
    bool elc;
    bool password;
    bool local_vault;
    bool online;
    bool one_party;

    void addPing(int64_t ping);
    int64_t getPing() const;
    bool isOffline() const;
    void reset();
    QStringList toStringList() const;


};

#endif // SERVER_H
