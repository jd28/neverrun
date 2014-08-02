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

#ifndef UTIL_H
#define UTIL_H

#include <QDebug>
#include <QStringList>

#include "server.h"

void errorMessage(const QString& err);
QString findUrl(const QString& str);
QString getPlayerCountFromDatagram(const QList<QByteArray> &datagram);
Server getServerFromDatagram(const QList<QByteArray> &datagram);
bool isUsableModuleCategory(const QString& str);
bool isUsableServerCategory(const QString& str);
bool isValidServerAddress(const QString& addr);
int RoomToGameType(const QString& room);
QString sanitizeName(const QString& str);

#endif // UTIL_H
