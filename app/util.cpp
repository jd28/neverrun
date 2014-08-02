#include <QMessageBox>

#include "util.h"

static int GetGameTypeFromServerDatagram(const QString& s) {
    if ( s == "274" ) { return 0; }
    else if ( s == "363" ) { return 1; }
    else if ( s == "364" ) { return 2; }
    else if ( s == "275" ) { return 3; }
    else if ( s == "276" ) { return 4; }
    else if ( s == "365" ) { return 5; }
    else if ( s == "366" ) { return 6; }
    else if ( s == "277" ) { return 7; }
    else if ( s == "279" ) { return 8; }
    else if ( s == "278" ) { return 9; }
    else if ( s == "367" ) { return 10; }
    else if ( s == "368" ) { return 11; }
    else if ( s == "370" ) { return 12; }

    return -1;
}

Server getServerFromDatagram(const QList<QByteArray> &datagram) {
    Server s;
    QString res;

    s.server_name = sanitizeName(datagram[3]);
    if (s.server_name.size() == 0) {
        s.server_name = "Unamed Server";
    }
    s.module_name = datagram[4];

    res = QString(datagram[5]);
    s.cur_players = res.toInt();
    res = QString(datagram[6]);
    s.max_players = res.toInt();
    res = QString(datagram[7]);
    s.min_level = res.toInt();
    res = QString(datagram[8]);
    s.max_level = res.toInt();
    res = QString(datagram[9]);
    if ( res == "NONE") {
        s.pvp = 0;
    }
    else if(res == "PARTY") {
        s.pvp = 1;
    }
    else if(res == "FULL") {
        s.pvp = 2;
    }

    res = datagram[10];
    s.password = res == "1";
    s.serv_description = datagram[15];
    s.gametype = GetGameTypeFromServerDatagram(datagram[16]);
    s.ilr = datagram[18] == "1";
    s.local_vault = datagram[19] == "1";
    s.elc = datagram[19] == "1";

    return s;
}

QString getPlayerCountFromDatagram(const QList<QByteArray> &datagram) {
    return datagram[5];
}

bool isUsableModuleCategory(const QString &str) {
    static QStringList cats = QStringList() << "All"
                                            << "Favorites";
    return !cats.contains(str);
}

bool isUsableServerCategory(const QString &str) {
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

int RoomToGameType(const QString &room) {
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

QString findUrl(const QString &str) {
    static QRegExp url("((?:https?)://\\S+)");
    url.setCaseSensitivity (Qt::CaseInsensitive);
    int n = url.indexIn(str);
    if ( n == -1 ) { return ""; }
    return url.cap(0);
}

QString sanitizeName(const QString &str) {
    static const QRegExp nonalpha("^[^a-zA-Z]*");
    QString s = str;
    s.remove(nonalpha);
    return s;
}

void errorMessage(const QString &err) {
    QMessageBox dlFailedMsgBox;
    dlFailedMsgBox.setIcon(QMessageBox::Critical);
    dlFailedMsgBox.setModal(true);
    dlFailedMsgBox.setWindowFlags(Qt::SplashScreen);
    dlFailedMsgBox.setText(QObject::tr("Error"));
    dlFailedMsgBox.setInformativeText(err);
    dlFailedMsgBox.exec();
}

bool isValidServerAddress(const QString &addr) {
    static QRegExp ip("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}:[0-9]{1,6}");
    int res = ip.indexIn(addr);
    return res != -1;
}
