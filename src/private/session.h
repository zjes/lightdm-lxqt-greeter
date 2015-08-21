#pragma once

#include <QVariant>

namespace priv {

class Greeter;

class Session
{
public:
    struct Info
    {
        QString key;
        QString type;
        QString name;
        QString comment;
    };

    enum class Type
    {
        Remote,
        Local
    };

public:
    Session(Greeter & greeter);
    static QList<Info> sessions(Type type);
    bool start(const QString &session);
    QString hint(const QString& user) const;
private:
    Greeter & m_greeter;
};

}

Q_DECLARE_METATYPE(priv::Session::Info)
