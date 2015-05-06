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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QObject>
#include <QFont>
#include <QStringList>
#include <QMap>

class Options : public QObject
{
    Q_OBJECT
public:
    enum ProxyMode { NoProxy, SystemProxy, ManualProxy };
    enum MarkdownConverter { DiscountMarkdownConverter, HoedownMarkdownConverter };

    explicit Options(QObject *parent = 0);

    void apply();

    /* Editor options */
    int tabWidth() const;
    void setTabWidth(int width);


    /* Internet options */
    ProxyMode proxyMode() const;
    void setProxyMode(ProxyMode mode);

    QString proxyHost() const;
    void setProxyHost(const QString &host);

    quint16 proxyPort() const;
    void setProxyPort(quint16 port);

    QString proxyUser() const;
    void setProxyUser(const QString &user);

    QString proxyPassword() const;
    void setProxyPassword(const QString &password);


    bool addServerCategory(const QString& cat);
    bool addModuleCategory(const QString& cat);

    /* Extra menu options */
    bool isAutolinkEnabled() const;
    void setAutolinkEnabled(bool enabled);

    bool isStrikethroughEnabled() const;
    void setStrikethroughEnabled(bool enabled);

    bool isAlphabeticListsEnabled() const;
    void setAlphabeticListsEnabled(bool enabled);

    bool isDefinitionListsEnabled() const;
    void setDefinitionListsEnabled(bool enabled);

    bool isSmartyPantsEnabled() const;
    void setSmartyPantsEnabled(bool enabled);

    bool isFootnotesEnabled() const;
    void setFootnotesEnabled(bool enabled);

    bool isSuperscriptEnabled() const;
    void setSuperscriptEnabled(bool enabled);

    bool isMathSupportEnabled() const;
    void setMathSupportEnabled(bool enabled);

    bool isCodeHighlightingEnabled() const;
    void setCodeHighlightingEnabled(bool enabled);

    bool isShowSpecialCharactersEnabled() const;
    void setShowSpecialCharactersEnabled(bool enabled);

    bool isWordWrapEnabled() const;
    void setWordWrapEnabled(bool enabled);

    bool isSpellingCheckEnabled() const;
    void setSpellingCheckEnabled(bool enabled);

    MarkdownConverter markdownConverter() const;
    void setMarkdownConverter(MarkdownConverter converter);

    void readSettings();
    void writeSettings();

    // Neverrun settings
    void setPassword(QString address, QString password, bool is_dm);
    QString getCurrentUserName() { return current_username_; }
    void setCurrentUserName(QString un);

    const QStringList& getUserNames() const { return usernames_; }
    bool addUserName(QString username) {
        if(usernames_.contains(username))
            return false;

        usernames_ << username;
        return true;
    }

    QString getServerLoader(const QString &addr) const;
    void setServerLoader(const QString &addr, const QString& file);
    QString getServerUpdater(const QString &addr) const;
    void setServerUpdater(const QString &addr, const QString& file);

    QStringList getCategoriesFromServer(const QString& address);
    QStringList getCategoriesFromModule(const QString& mod);
signals:
    void tabWidthChanged(int tabWidth);
    void proxyConfigurationChanged();
    void markdownConverterChanged();

public:
    QString cdkey;
    QString player_name;
    QString m_NWN_path;

    QString getNWNPath() { return m_NWN_path; }
    void addDirectConnect(const QString &addr);
    QString getPassword(QString address, bool is_dm) const;
    QStringList getCategoryIPs(const QString &cat) const;
    QStringList getCategoryModules(const QString &cat) const;
    int getClientPort() const;
    QStringList getDirectConnects();
    QStringList getModuleCategories() const;
    QStringList getServerCategories() const;
    void removeServerCategory(const QString& cat);
    void addServerToCategory(const QString& cat, const QString& addr);
    void removeServerFromCategory(const QString& cat, const QString& addr);
    void addModuleToCategory(const QString& cat, const QString& addr);
    void removeModuleCategory(const QString &cat);
    void removeModuleFromCategory(const QString& cat, const QString& addr);
    void removeUserName(const QString& name);
    QStringList getBlacklist();
    void addToBlacklist(const QString& ip);
    void removeFromBlacklist(const QString& ip);

    QString getDefaultLoader() { return default_loader_; }
    QString getDefaultToolset() { return default_toolset_; }
    void setDefaultLoader(const QString& val) { default_loader_ = val; }
    void setDefaultToolset(const QString& val) { default_toolset_ = val; }

    bool getUpdateBackground() { return update_background_; }
    void setUpdateBackground(bool val) { update_background_ = val; }

private:
    QFont font;
    int m_tabWidth;
    ProxyMode m_proxyMode;
    QString m_proxyHost;
    quint16 m_proxyPort;
    QString m_proxyUser;
    QString m_proxyPassword;
    bool m_autolinkEnabled;
    bool m_strikethroughEnabled;
    bool m_alphabeticListsEnabled;
    bool m_definitionListsEnabled;
    bool m_smartyPantsEnabled;
    bool m_footnotesEnabled;
    bool m_superscriptEnabled;
    bool m_mathSupportEnabled;
    bool m_codeHighlightingEnabled;
    bool m_showSpecialCharactersEnabled;
    bool m_wordWrapEnabled;
    bool m_spellingCheckEnabled;
    QString m_dictionaryLanguage;
    MarkdownConverter m_markdownConverter;
    QString current_username_;
    QStringList usernames_;
    QStringList direct_connect_;
    QStringList blacklist_;
    QMap<QString, QStringList> server_categories_;
    QMap<QString, QStringList> module_categories_;
    QMap<QString, QString> password_dm_;
    QMap<QString, QString> password_player_;
    QMap<QString, QString> server_loader_;
    QMap<QString, QString> server_updater_;
    int client_port_;

    QString default_loader_;
    QString default_toolset_;
    bool update_background_;

};

#endif // OPTIONS_H
