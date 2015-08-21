#pragma once
#include <QtDBus/QDBusInterface>

namespace priv {

class Power
{
public:
    Power();

public:
    bool canSuspend() const;
    bool canHibernate() const;
    bool canShutdown() const;
    bool canRestart() const;

public:
    bool suspend();
    bool hibernate();
    bool shutdown();
    bool restart();
private:
    QScopedPointer<QDBusInterface> m_pmInterface;
    QScopedPointer<QDBusInterface> m_ckInterface;
    QScopedPointer<QDBusInterface> m_interface;
};

}
