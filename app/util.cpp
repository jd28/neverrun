#include <QMessageBox>
#include <QColorDialog>
#include <QFile>
#include <qt_windows.h>
#include <QDebug>
#include <yaml-cpp/yaml.h>

#include "util.h"

static int GetGameTypeFromServerDatagram(const QString& s) {
    int n = s.toInt();
    switch(n) {
    default: return -1;
    case 274: return 0;
    case 363: return 1;
    case 364: return 2;
    case 275: return 3;
    case 276: return 4;
    case 365: return 5;
    case 366: return 6;
    case 277: return 7;
    case 279: return 8;
    case 278: return 9;
    case 367: return 10;
    case 368: return 11;
    case 370: return 12;
    }
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

void errorMessage(const QString &err, QWidget *parent) {
    QMessageBox dlFailedMsgBox(parent);
    dlFailedMsgBox.setIcon(QMessageBox::Critical);
    dlFailedMsgBox.setModal(true);
    dlFailedMsgBox.setWindowFlags(Qt::SplashScreen);
    dlFailedMsgBox.setText(QObject::tr("Error"));
    dlFailedMsgBox.setInformativeText(err);
    dlFailedMsgBox.exec();
}

bool isValidServerAddress(const QString &addr, bool port) {
    static QRegExp ip("^([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})(:[0-9]{1,6})?$");
    static QRegExp ipp("^([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})(:[0-9]{1,6})$");

    int res = port ? ipp.indexIn(addr) : ip.indexIn(addr);
    return res != -1;
}

uint64_t getTickCount() {
#ifdef Q_OS_WIN
    return GetTickCount();
#endif
    return 0;
}

bool readPacket(const QByteArray &data, Server &s) {
    bool updated = false;
    if ( data.size() >= 20 && data.mid(0, 4) == "BNXR") {
        updated = parseBNXR(data, s) || updated;
    }
    else if ( data.size() >= 9 && data.mid(0, 5) == "BNERU") {
        updated = parseBNER(data, s) || updated;
    }
    else if ( data.size() >= 10 && data.mid(0, 4) == "BNDR") {
        updated = parseBNDR(data, s) || updated;
    }
    return updated;
}

bool parseBNDR(const QByteArray &data, Server &s) {
    const char *d = data.data();

    bool updated = false;
    int offset = 6;

    uint32_t sdesc_size = 0;
    memcpy(&sdesc_size, d + offset, 4);
    offset += 4;
    if ( sdesc_size != 0 ) {
        if ( sdesc_size != s.serv_description.size() ) {
            s.serv_description = data.mid(offset, sdesc_size);
            auto lines = s.serv_description.split(QRegExp("\n|\r\n|\r"));

            QString yaml;
            if (lines[0].startsWith("---")) {
                for(int i = 1; i < lines.size(); ++i) {
                    if(lines[i].startsWith("---")) {
                        lines.erase(lines.begin(), lines.begin()+i+1);
                        s.serv_description = lines.join("");
                        break;
                    }
                    yaml += lines[i] + "\n";
                }
            }

            YAML::Node node;
            if(yaml.size() > 0) {
                try {
                    node = YAML::Load(yaml.toUtf8());
                    if(node["website"]) {
                        s.homepage = QString::fromStdString(node["website"].as<std::string>());
                    }
                    if(node["forum"]) {
                        s.forum = QString::fromStdString(node["forum"].as<std::string>());
                    }
                }
                catch(...) {
                    qDebug() << "Failed to load yaml for server.";
                }
            }
            updated = true;
        }
        offset += sdesc_size;
    }

    int mdesc_size = 0;
    memcpy(&mdesc_size, d + offset, 4);
    offset += 4;
    if ( mdesc_size != 0 ) {
        if ( mdesc_size != s.mod_description.size() ) {
            s.mod_description = data.mid(offset, mdesc_size);
            if ( s.homepage.size() == 0 ) {
                QString url = findUrl(s.mod_description);
                if (url.size() > 0) {
                    if ( url.endsWith(".nrl", Qt::CaseInsensitive)) { s.nrl = url; }
                    else { s.homepage = url; }
                }
            }
            updated = true;
        }
        offset += mdesc_size;
    }

    offset += 8; // Don't care about build number...
    if ( s.gametype != data[offset] ) {
        s.gametype = data[offset];
        updated = true;
    }

    s.messages_received |= SERVER_MESSAGES_RECIEVED_BNDR;
    return updated;
}


bool parseBNER(const QByteArray &data, Server &s) {
    if(s.messages_received & SERVER_MESSAGES_RECIEVED_BNER) { return false; }
    const char *d = data.data();

    bool updated = false;
    int offset = 8;
    uint32_t size = d[offset];
    offset += 1;
    if ( size > 0 ) {
        s.server_name = sanitizeName(data.mid(offset, size));
        if ( s.server_name.length() == 0 ) {
            s.server_name = "Unnamed Server";
        }
        updated = true;
    }

    s.messages_received |= SERVER_MESSAGES_RECIEVED_BNER;
    return updated;
}


bool parseBNXR(const QByteArray &data, Server &s) {
    bool updated = false;

#define S(member, idx) \
    if((int)member != (uchar)data.at(idx)) { updated = true; member = (uchar)data.at(idx); }

    S(s.password, 7);
    S(s.min_level, 8);
    S(s.max_level, 9);
    S(s.cur_players, 10);
    S(s.max_players, 11);
    S(s.local_vault, 12);
    S(s.pvp, 13);
    S(s.one_party, 15);
    S(s.elc, 16);
    S(s.ilr, 17);

    int mod_name_size = data[19];
    if (mod_name_size != 0 && mod_name_size != s.module_name.size()) {
        s.module_name = data.mid(20, mod_name_size);
        updated = true;
    }

    s.messages_received |= SERVER_MESSAGES_RECIEVED_BNXR;

    return updated;
#undef S
}

bool FileConvert20ToSpace(QString filename) {
    QFile file;
    file.setFileName(filename);
    if (file.open(QIODevice::ReadWrite) == false)
        return false;

    QList<QByteArray> bytearrayList;
    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        line.replace("%20", " ");
        line.replace(", ", ",");
        bytearrayList.append(line);
    }
    file.close();
    QFile savefile;
    savefile.setFileName(filename);
    if (savefile.open(QIODevice::WriteOnly) == false)
        return false;

    while(!bytearrayList.empty()) {
        QByteArray &line = bytearrayList.front();
        savefile.write(line);
        bytearrayList.pop_front();
    }
    savefile.close();
    return true;
}

QColor getBackgroundColor(const QColor& c) {
    QColorDialog dlg;
    QColor color = dlg.getColor(c);
    return color;
}

QString getBackgroundColorSS(const QColor& color) {
    return QString("background-color: #%1").arg(color.rgb(), 1, 16);
}

QString getBackgroundColorText(const QColor& color) {
    return QString("%1,%2,%3").arg(QString::number(color.red()),
                                   QString::number(color.green()),
                                   QString::number(color.blue()));
}


QColor getBackgroundColorFromString(const QString& col) {
    QStringList cs = col.split(',');
    return QColor(cs[0].toInt(), cs[1].toInt(), cs[2].toInt());
}
