#pragma once

#include <QString>
#include <QScopedPointer>
#include "user.h"
#include "session.h"

namespace pimpl
{ class Greeter; }

class Greeter: public QObject
{
    Q_OBJECT
public:
    Greeter();
    ~Greeter();
public:
    enum class Prompt
    {
        Question,
        TypeSecret
    };

    enum class Message
    {
        Info,
        Error
    };

    friend class Users;
    friend class Session;
public:
    Users & users()
    { return m_users; }

    Session & session()
    { return m_session; }

public:
    bool connectToLightDM();
    static QString hostName();
signals:
    void showPrompt(const QString& prompt, Prompt promptType);
    void showMessage(const QString& msg, Message msgType);
    void authenticationComplete();
    void reset();
private:
    QScopedPointer<pimpl::Greeter> m_pimpl;
    Users                          m_users;
    Session                        m_session;
};


