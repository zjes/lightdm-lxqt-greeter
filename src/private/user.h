#pragma once

#include <QMap>
#include <QVariant>

namespace priv {

class Greeter;

class Users
{
public:
    struct User
    {
        QString displayName;
        QString homeDirectory;
        QString image;
        QString background;
        QString language;
        QString layout;
        bool    loggedIn;
        QString name;
        QString realName;
        QString session;
        quint64 uid;
    };
public:
    Users(Greeter & greeter);
public:
    static const std::map<QString, User> & users();
public:
    QString hint() const;
    void authenticate(const QString& user);
    bool isAuthenticated() const;
    void cancelAuthentication();
    void authorize(const QString& pwd);
    const User & authenticationUser() const;
    bool inAuthentication() const;

    QString sharedDataDirSync(const QString &username);
private:
    Greeter & m_greeter;
};

}

Q_DECLARE_METATYPE(priv::Users::User)
