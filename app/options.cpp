/*
 * Copyright 2013 Christian Loose <christian.loose@hamburg.de>
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

#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QStringList>

#include "options.h"

static const char* MARKDOWN_CONVERTER = "general/converter";
static const char* FONT_FAMILY_DEFAULT = "Monospace";
static const char* FONT_FAMILY = "editor/font/family";
static const char* FONT_SIZE = "editor/font/size";
static const char* TAB_WIDTH = "editor/tabwidth";
static const char* PROXY_MODE = "internet/proxy/mode";
static const char* PROXY_HOST = "internet/proxy/host";
static const char* PROXY_PORT = "internet/proxy/port";
static const char* PROXY_USER = "internet/proxy/user";
static const char* PROXY_PASSWORD = "internet/proxy/password";
static const char* AUTOLINK_ENABLED = "extensions/autolink";
static const char* STRIKETHROUGH_ENABLED = "extensions/strikethrough";
static const char* ALPHABETICLISTS_ENABLED = "extensions/alphabeticLists";
static const char* DEFINITIONSLISTS_ENABLED = "extensions/definitionLists";
static const char* SMARTYPANTS_ENABLED = "extensions/smartyPants";
static const char* FOOTNOTES_ENABLED = "extensions/footnotes";
static const char* SUPERSCRIPT_ENABLED = "extensions/superscript";
static const char* MATHSUPPORT_ENABLED = "mathsupport/enabled";
static const char* CODEHIGHLIGHT_ENABLED = "codehighlighting/enabled";
static const char* SHOWSPECIALCHARACTERS_ENABLED = "specialchars/enabled";
static const char* WORDWRAP_ENABLED = "wordwrap/enabled";

static const char* DIRECT_CONNECT = "DirectConnect";
static const char* SERVER_LOADERS = "ServerLoaders";
static const char* SERVER_UPDATERS = "ServerUpdaters";

Options::Options(QObject *parent) :
    QObject(parent)
{
    m_NWN_path = "../";
#ifdef _WIN32
    QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\BioWare\\NWN\\Neverwinter", QSettings::NativeFormat);
    QString path = settings.value("Location").toString();
    qDebug() << path;
    if ( path.size() > 0 ) {
        m_NWN_path = path;
    }
    else {
        QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\BioWare\\NWN\\Neverwinter", QSettings::NativeFormat);
        path = settings.value("Location").toString();
        qDebug() << path;
        if ( path.size() > 0 ) {
            m_NWN_path = path;
        }
    }
#endif

}

void Options::apply()
{
    emit proxyConfigurationChanged();
    emit markdownConverterChanged();
}

int Options::tabWidth() const
{
    return m_tabWidth;
}

void Options::setTabWidth(int width)
{
    m_tabWidth = width;
    emit tabWidthChanged(width);
}

QString Options::getPassword(QString address, bool is_dm) const {
    if (is_dm) {
        auto it = password_dm_.find(address);
        if (it != password_dm_.end())
            return it.value();
    }
    else {
        auto it = password_player_.find(address);
        if (it != password_player_.end())
            return it.value();
    }
    return "";
}

void Options::setPassword(QString address, QString password, bool is_dm) {
    if (is_dm) {
        if ( password.size() == 0 ) { password_dm_.remove(address); return; }
        password_dm_[address] = password;
    }
    else {
        if ( password.size() == 0 ) { password_player_.remove(address); return; }
        password_player_[address] = password;
    }
}

QString Options::getServerLoader(const QString &addr) const {
    auto it = server_loader_.find(addr);
    if ( it == server_loader_.end()) { return ""; }
    return it.value();
}

void Options::setServerLoader(const QString &addr, const QString &file) {
    if ( file.size() == 0 ) { server_loader_.remove(addr); return; }
    server_loader_[addr] = file;
}

QString Options::getServerUpdater(const QString& addr) const {
    auto it = server_updater_.find(addr);
    if ( it == server_updater_.end()) { return ""; }
    return it.value();
}

void Options::setServerUpdater(const QString& addr, const QString &file) {
    if ( file.size() == 0 ) { server_updater_.remove(addr); return; }
    server_updater_[addr] = file;
}

Options::ProxyMode Options::proxyMode() const
{
    return m_proxyMode;
}

void Options::setProxyMode(Options::ProxyMode mode)
{
    m_proxyMode = mode;
}

QString Options::proxyHost() const
{
    return m_proxyHost;
}

void Options::setProxyHost(const QString &host)
{
    m_proxyHost = host;
}

quint16 Options::proxyPort() const
{
    return m_proxyPort;
}

void Options::setProxyPort(quint16 port)
{
    m_proxyPort = port;
}

QString Options::proxyUser() const
{
    return m_proxyUser;
}

void Options::setProxyUser(const QString &user)
{
    m_proxyUser = user;
}

QString Options::proxyPassword() const
{
    return m_proxyPassword;
}

void Options::setProxyPassword(const QString &password)
{
    m_proxyPassword = password;
}

static bool FileConvert20ToSpace(QString filename) {
    QFile file;
    file.setFileName(filename);
    if (file.open(QIODevice::ReadWrite) == false)
        return false;

    QList<QByteArray> bytearrayList;
    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        line.replace("%20", " ");
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

void Options::addDirectConnect(const QString& addr) {
    if( addr.size() > 0 && !direct_connect_.contains(addr) ) {
        direct_connect_ << addr;
    }
}

QStringList Options::getDirectConnects() {
    return direct_connect_;
}

QStringList Options::getCategoryIPs(const QString& cat) const {
    auto it = server_categories_.find(cat);
    if (it != server_categories_.end()) {
        return it.value();
    }
    return QStringList();
}

QStringList Options::getCategoryModules(const QString &cat) const {
    auto it = module_categories_.find(cat);
    if (it != module_categories_.end()) {
        return it.value();
    }
    return QStringList();
}

QStringList Options::getServerCategories() const {
    return server_categories_.keys();
}

QStringList Options::getModuleCategories() const {
    return module_categories_.keys();
}

void Options::removeServerCategory(const QString &cat) {
    QSettings settings(QDir::homePath() + "/.neverrun/neverrun.ini", QSettings::IniFormat);
    settings.remove("Server_"+cat);
    server_categories_.remove(cat);
}

void Options::removeModuleCategory(const QString &cat) {
    QSettings settings(QDir::homePath() + "/.neverrun/neverrun.ini", QSettings::IniFormat);
    settings.remove("Module_"+cat);
    module_categories_.remove(cat);

}

void Options::addServerToCategory(const QString &cat, const QString &addr) {
    addServerCategory(cat);
    if ( server_categories_[cat].contains(addr) ) {
        return;
    }
    server_categories_[cat] << addr;
}

void Options::removeServerFromCategory(const QString &cat, const QString &addr) {
    server_categories_[cat].removeAll(addr);
}

void Options::addModuleToCategory(const QString &cat, const QString &addr) {
    addModuleCategory(cat);
    if ( module_categories_[cat].contains(addr) ) {
        return;
    }
    module_categories_[cat] << addr;
}

void Options::removeModuleFromCategory(const QString &cat, const QString &addr) {
    module_categories_[cat].removeAll(addr);
}

void Options::removeUserName(const QString &name) {
    usernames_.removeAll(name);
}

void Options::setCurrentUserName(QString un) {
     current_username_ = un;
     QSettings nwn_settings(m_NWN_path + "/nwnplayer.ini", QSettings::IniFormat);
     nwn_settings.setValue("Profile/Player Name", un);
     nwn_settings.sync();
     FileConvert20ToSpace(m_NWN_path + "/nwnplayer.ini");
}

QStringList Options::getCategoriesFromServer(const QString &address) {
    QStringList res;

    auto i = server_categories_.begin();
    while (i != server_categories_.end()) {
        if (i.value().contains(address)) {
            res << i.key();
        }
        ++i;
    }
    return res;
}

QStringList Options::getCategoriesFromModule(const QString &mod) {
    QStringList res;

    auto i = module_categories_.begin();
    while (i != module_categories_.end()) {
        if (i.value().contains(mod)) {
            res << i.key();
        }
        ++i;
    }
    return res;
}

bool Options::addServerCategory(const QString &cat) {
    if (server_categories_.find(cat) != server_categories_.end()) {
        return false;
    }
    server_categories_.insert(cat, QStringList());
    return true;
}

bool Options::addModuleCategory(const QString &cat) {
    if (module_categories_.find(cat) != module_categories_.end()) {
        return false;
    }
    module_categories_.insert(cat, QStringList());
    return true;
}

bool Options::isAutolinkEnabled() const
{
    return m_autolinkEnabled;
}

void Options::setAutolinkEnabled(bool enabled)
{
    m_autolinkEnabled = enabled;
}

bool Options::isStrikethroughEnabled() const
{
    return m_strikethroughEnabled;
}

void Options::setStrikethroughEnabled(bool enabled)
{
    m_strikethroughEnabled = enabled;
}

bool Options::isAlphabeticListsEnabled() const
{
    return m_alphabeticListsEnabled;
}

void Options::setAlphabeticListsEnabled(bool enabled)
{
    m_alphabeticListsEnabled = enabled;
}

bool Options::isDefinitionListsEnabled() const
{
    return m_definitionListsEnabled;
}

void Options::setDefinitionListsEnabled(bool enabled)
{
    m_definitionListsEnabled = enabled;
}

bool Options::isSmartyPantsEnabled() const
{
    return m_smartyPantsEnabled;
}

void Options::setSmartyPantsEnabled(bool enabled)
{
    m_smartyPantsEnabled = enabled;
}

bool Options::isFootnotesEnabled() const
{
    return m_footnotesEnabled;
}

void Options::setFootnotesEnabled(bool enabled)
{
    m_footnotesEnabled = enabled;
}

bool Options::isSuperscriptEnabled() const
{
    return m_superscriptEnabled;
}

void Options::setSuperscriptEnabled(bool enabled)
{
    m_superscriptEnabled = enabled;
}

bool Options::isMathSupportEnabled() const
{
    return m_mathSupportEnabled;
}

void Options::setMathSupportEnabled(bool enabled)
{
    m_mathSupportEnabled = enabled;
}

bool Options::isCodeHighlightingEnabled() const
{
    return m_codeHighlightingEnabled;
}

void Options::setCodeHighlightingEnabled(bool enabled)
{
    m_codeHighlightingEnabled = enabled;
}

bool Options::isShowSpecialCharactersEnabled() const
{
    return m_showSpecialCharactersEnabled;
}

void Options::setShowSpecialCharactersEnabled(bool enabled)
{
    m_showSpecialCharactersEnabled = enabled;
}

bool Options::isWordWrapEnabled() const
{
    return m_wordWrapEnabled;
}

void Options::setWordWrapEnabled(bool enabled)
{
    m_wordWrapEnabled = enabled;
}

bool Options::isSpellingCheckEnabled() const
{
    return m_spellingCheckEnabled;
}

void Options::setSpellingCheckEnabled(bool enabled)
{
    m_spellingCheckEnabled = enabled;
}

Options::MarkdownConverter Options::markdownConverter() const
{
    return m_markdownConverter;
}

void Options::setMarkdownConverter(Options::MarkdownConverter converter)
{
    if (m_markdownConverter != converter) {
        m_markdownConverter = converter;
        emit markdownConverterChanged();
    }
}

void Options::readSettings()
{
    QSettings nwn_settings(m_NWN_path + "/nwnplayer.ini", QSettings::IniFormat);
    current_username_ = nwn_settings.value("Profile/Player Name").toString();

    QSettings settings(QDir::homePath() + "/.neverrun/neverrun.ini", QSettings::IniFormat);

    // general settings
    m_markdownConverter = (Options::MarkdownConverter)settings.value(MARKDOWN_CONVERTER, 0).toInt();

    m_tabWidth = settings.value(TAB_WIDTH, 8).toInt();

    // proxy settings
    m_proxyMode = (Options::ProxyMode)settings.value(PROXY_MODE, 0).toInt();
    m_proxyHost = settings.value(PROXY_HOST, "").toString();
    m_proxyPort = settings.value(PROXY_PORT, 0).toInt();
    m_proxyUser = settings.value(PROXY_USER, "").toString();
    m_proxyPassword = settings.value(PROXY_PASSWORD, "").toString();

    // extension settings
    m_autolinkEnabled = settings.value(AUTOLINK_ENABLED, true).toBool();
    m_strikethroughEnabled = settings.value(STRIKETHROUGH_ENABLED, true).toBool();
    m_alphabeticListsEnabled = settings.value(ALPHABETICLISTS_ENABLED, true).toBool();
    m_definitionListsEnabled = settings.value(DEFINITIONSLISTS_ENABLED, true).toBool();
    m_smartyPantsEnabled = settings.value(SMARTYPANTS_ENABLED, true).toBool();
    m_footnotesEnabled = settings.value(FOOTNOTES_ENABLED, true).toBool();
    m_superscriptEnabled = settings.value(SUPERSCRIPT_ENABLED, true).toBool();

    m_mathSupportEnabled = settings.value(MATHSUPPORT_ENABLED, false).toBool();
    m_codeHighlightingEnabled = settings.value(CODEHIGHLIGHT_ENABLED, false).toBool();
    m_showSpecialCharactersEnabled = settings.value(SHOWSPECIALCHARACTERS_ENABLED, false).toBool();
    m_wordWrapEnabled = settings.value(WORDWRAP_ENABLED, true).toBool();

    settings.beginGroup("DMPasswords");
    QStringList keys = settings.allKeys();
    foreach (QString key, keys) {
        password_dm_[key] = settings.value(key).toString();
    }
    settings.endGroup();

    settings.beginGroup(SERVER_LOADERS);
    keys = settings.allKeys();
    foreach (QString key, keys) {
        server_loader_[key] = settings.value(key).toString();
    }
    settings.endGroup();

    settings.beginGroup(SERVER_UPDATERS);
    keys = settings.allKeys();
    foreach (QString key, keys) {
        server_updater_[key] = settings.value(key).toString();
    }
    settings.endGroup();

    settings.beginGroup("PlayerPasswords");
    keys = settings.childKeys();
    foreach (QString key, keys) {
        password_player_[key] = settings.value(key).toString();
    }
    settings.endGroup();

    int size = settings.beginReadArray("UserNames");
    for(int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        usernames_ << settings.value("name").toString();
    }
    settings.endArray();

    QStringList cats;
    size = settings.beginReadArray("ServerCategories");
    for(int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        cats << settings.value("name").toString();
    }
    settings.endArray();

    size = settings.beginReadArray(DIRECT_CONNECT);
    for(int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        direct_connect_ << settings.value("address").toString();
    }
    settings.endArray();

    foreach(const QString& c, cats) {
        QStringList servers;
        int size = settings.beginReadArray("Server_"+c);
        for(int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);
            servers << settings.value("server").toString();
        }
        settings.endArray();
        server_categories_.insert(c, servers);
    }

    cats.clear();
    size = settings.beginReadArray("ModuleCategories");
    for(int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        cats << settings.value("name").toString();
    }
    settings.endArray();

    foreach(const QString& c, cats) {
        QStringList servers;
        int size = settings.beginReadArray("Module_"+c);
        for(int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);
            servers << settings.value("module").toString();
        }
        settings.endArray();
        module_categories_.insert(c, servers);
    }

    if ( module_categories_.find("Favorites") == module_categories_.end()) {
        module_categories_.insert("Favorites", QStringList());
    }

    if (server_categories_.find("History") == server_categories_.end()) {
        server_categories_.insert("History", QStringList());
    }
    if (server_categories_.find("Favorites") == server_categories_.end()) {
        server_categories_.insert("Favorites", QStringList());
    }

    QVariant h = nwn_settings.value("History Page/server0");
    int i = 1;
    while(h.isValid() && h.toString().size() > 0) {
        QString s = h.toString();
        if ( !server_categories_["History"].contains(s) ) {
            server_categories_["History"] << s;
        }
        h = nwn_settings.value("History Page/server" + QString::number(i++));
    }

    h = nwn_settings.value("Favorites Page/server0");
    i = 1;
    while(h.isValid() && h.toString().size() > 0) {
        QString s = h.toString();
        if ( !server_categories_["Favorites"].contains(s) ) {
            server_categories_["Favorites"] << s;
        }
    }

    if(!usernames_.contains(current_username_)) {
        usernames_ << current_username_;
    }

    usernames_.sort(Qt::CaseInsensitive);



    apply();
}

void Options::writeSettings()
{
    QSettings settings(QDir::homePath() + "/.neverrun/neverrun.ini", QSettings::IniFormat);

    // general settings
    settings.setValue(MARKDOWN_CONVERTER, m_markdownConverter);

    // editor settings
    settings.setValue(TAB_WIDTH, m_tabWidth);

    // proxy settings
    settings.setValue(PROXY_MODE, m_proxyMode);
    settings.setValue(PROXY_HOST, m_proxyHost);
    settings.setValue(PROXY_PORT, m_proxyPort);
    settings.setValue(PROXY_USER, m_proxyUser);
    settings.setValue(PROXY_PASSWORD, m_proxyPassword);

    // extensions settings
    settings.setValue(AUTOLINK_ENABLED, m_autolinkEnabled);
    settings.setValue(STRIKETHROUGH_ENABLED, m_strikethroughEnabled);
    settings.setValue(ALPHABETICLISTS_ENABLED, m_alphabeticListsEnabled);
    settings.setValue(DEFINITIONSLISTS_ENABLED, m_definitionListsEnabled);
    settings.setValue(SMARTYPANTS_ENABLED, m_smartyPantsEnabled);
    settings.setValue(FOOTNOTES_ENABLED, m_footnotesEnabled);
    settings.setValue(SUPERSCRIPT_ENABLED, m_superscriptEnabled);

    settings.setValue(MATHSUPPORT_ENABLED, m_mathSupportEnabled);
    settings.setValue(CODEHIGHLIGHT_ENABLED, m_codeHighlightingEnabled);
    settings.setValue(SHOWSPECIALCHARACTERS_ENABLED, m_showSpecialCharactersEnabled);
    settings.setValue(WORDWRAP_ENABLED, m_wordWrapEnabled);


    settings.beginGroup("DMPasswords");
    for (auto it = password_dm_.constBegin(); it != password_dm_.constEnd(); ++it) {
         settings.setValue(it.key(), it.value());
     }
    settings.endGroup();

    settings.beginGroup(SERVER_LOADERS);
    for(auto i = server_loader_.constBegin(); i != server_loader_.constEnd(); ++i) {
         settings.setValue(i.key(), i.value());
     }
    settings.endGroup();

    settings.beginGroup(SERVER_UPDATERS);
    for (auto i = server_updater_.constBegin(); i != server_updater_.constEnd(); ++i) {
         settings.setValue(i.key(), i.value());
     }
    settings.endGroup();

    settings.beginGroup("PlayerPasswords");
    auto it = password_player_.constBegin();
    while (it != password_player_.constEnd()) {
         settings.setValue(it.key(), it.value());
         ++it;
     }
    settings.endGroup();

    settings.beginWriteArray("UserNames", usernames_.size());
    for(int i = 0; i < usernames_.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("name", usernames_[i]);
    }
    settings.endArray();

    settings.beginWriteArray(DIRECT_CONNECT, direct_connect_.size());
    for(int i = 0; i < direct_connect_.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("address", direct_connect_[i]);
    }
    settings.endArray();

    QStringList cats = server_categories_.keys();
    settings.beginWriteArray("ServerCategories", cats.size());
    for(int i = 0; i < cats.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("name", cats[i]);
    }
    settings.endArray();

    foreach(const QString& c, cats) {
        auto it = server_categories_.find(c);
        if ( it == server_categories_.end() ) { continue; }

        settings.beginWriteArray("Server_"+c);
        QStringList& ls = it.value();
        for(int i = 0; i < ls.size(); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("server", ls[i]);
        }
        settings.endArray();
    }

    cats = module_categories_.keys();
    settings.beginWriteArray("ModuleCategories", cats.size());
    for(int i = 0; i < cats.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("name", cats[i]);
    }
    settings.endArray();

    foreach(const QString& c, cats) {
        auto it = module_categories_.find(c);
        if ( it == module_categories_.end() ) { continue; }

        settings.beginWriteArray("Module_"+c);
        QStringList& ls = it.value();
        for(int i = 0; i < ls.size(); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("module", ls[i]);
        }
        settings.endArray();
    }
}
