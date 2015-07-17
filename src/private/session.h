#pragma once

#include <QVariant>

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
    QString hint() const;
private:
    Greeter & m_greeter;
};


Q_DECLARE_METATYPE(Session::Info)
