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

#include <QStringList>

static int RoomToGameType(const QString& room) {
    // The room list
    if (room == "Action") return 0;
    if (room == "Role Play") return 3;
    if (room == "Team") return 4;
    if (room == "Social") return 7;
    if (room == "PW Action") return 9;
    if (room == "Alternative") return 8;
    if (room == "Story") return 1;
    if (room == "Story Lite") return 2;
    if (room == "Melee") return 5;
    if (room == "Arena") return 6;
    if (room == "PW Story") return 10;
    if (room == "Solo") return 11;
    if (room == "Tech Support") return 12;

    return -1; //Handle differently;

}

static bool isUsableServerCategory(const QString& str) {
    static QStringList cats = QStringList() << "All"
                                            << "History"
                                            << "Favorites"
                                            << "Action"
                                            << "Role Play"
                                            << "Team"
                                            << "Social"
                                            << "PW Action"
                                            << "Alternative"
                                            << "Story"
                                            << "Story Lite"
                                            << "Melee"
                                            << "Arena"
                                            << "PW Story"
                                            << "Solo"
                                            << "Tech Support";

    return !cats.contains(str);
}

static bool isUsableModuleCategory(const QString& str) {
    static QStringList cats = QStringList() << "All"
                                            << "Favorites";
    return !cats.contains(str);
}

#endif // UTIL_H
