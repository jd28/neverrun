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

void errorMessage(const QString& err, QWidget *parent = 0);
QString findUrl(const QString& str);
QString getPlayerCountFromDatagram(const QList<QByteArray> &datagram);
Server getServerFromDatagram(const QList<QByteArray> &datagram);
bool isUsableModuleCategory(const QString& str);
bool isUsableServerCategory(const QString& str);
bool isValidServerAddress(const QString& addr, bool port);
int RoomToGameType(const QString& room);
QString sanitizeName(const QString& str);
bool parseBNDR(const QByteArray& data, Server& s);
bool parseBNER(const QByteArray& data, Server& s);
bool parseBNXR(const QByteArray& data, Server& s);
bool readPacket(const QByteArray& data, Server& s);
uint64_t getTickCount();
bool FileConvert20ToSpace(QString filename);
QColor getBackgroundColor(const QColor& c);
QString getBackgroundColorSS(const QColor& color);
QString getBackgroundColorText(const QColor& color);
QColor getBackgroundColorFromString(const QString &col);
#endif // UTIL_H
