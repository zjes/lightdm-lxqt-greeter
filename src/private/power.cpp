#include <QDebug>
#include <QtDBus/QDBusReply>
#include "power.h"

namespace priv {

Power::Power():
    m_pmInterface(new QDBusInterface("org.freedesktop.UPower","/org/freedesktop/UPower", "org.freedesktop.UPower", QDBusConnection::systemBus())),
    m_ckInterface(new QDBusInterface("org.freedesktop.ConsoleKit", "/org/freedesktop/ConsoleKit/Manager", "org.freedesktop.ConsoleKit.Manager", QDBusConnection::systemBus())),
    m_interface(new QDBusInterface("org.freedesktop.login1", "/org/freedesktop/login1", "org.freedesktop.login1.Manager", QDBusConnection::systemBus()))
{
}

bool Power::canSuspend() const
{
    if (m_interface->isValid()){
        QDBusReply<QString> reply = m_interface->call("CanSuspend");
        if (reply.isValid())
            return reply.value() == "yes";
    }

    qWarning() << m_interface->lastError();

    QDBusReply<bool> reply = m_pmInterface->call("SuspendAllowed");
    if (reply.isValid())
        return reply.value();
    return false;
}

bool Power::canHibernate() const
{
    if (m_interface->isValid()) {
        QDBusReply<QString> reply = m_interface->call("CanHibernate");
        if (reply.isValid())
            return reply.value() == "yes";
    }

    qWarning() << m_interface->lastError();

    QDBusReply<bool> reply = m_pmInterface->call("HibernateAllowed");
    if (reply.isValid())
        return reply.value();

    return false;
}

bool Power::canShutdown() const
{
    if (m_interface->isValid()) {
        QDBusReply<QString> reply = m_interface->call("CanPowerOff");
        if (reply.isValid())
            return reply.value() == "yes";
    }

    qWarning() << m_interface->lastError();

    QDBusReply<bool> reply = m_ckInterface->call("CanStop");
    if (reply.isValid())
        return reply.value();

    return false;
}

bool Power::canRestart() const
{
    if (m_interface->isValid()) {
        QDBusReply<QString> reply = m_interface->call("CanReboot");
        if (reply.isValid())
            return reply.value() == "yes";
    }
    qWarning() << m_interface->lastError();

    QDBusReply<bool> reply = m_ckInterface->call("CanRestart");
    if (reply.isValid())
        return reply.value();

    return false;
}

bool Power::suspend()
{
    QDBusReply<void> reply;
    if (m_interface->isValid())
        reply = m_interface->call("Suspend", false);
    else
        reply = m_pmInterface->call("Suspend");

    return reply.isValid ();
}

bool Power::hibernate()
{
    QDBusReply<void> reply;
    if (m_interface->isValid())
        reply = m_interface->call("Hibernate", false);
    else
        reply = m_pmInterface->call("Hibernate");

    return reply.isValid ();
}

bool Power::shutdown()
{
    QDBusReply<void> reply;
    if (m_interface->isValid())
        reply = m_interface->call("PowerOff", false);
    else
        reply = m_ckInterface->call("Stop");
    return reply.isValid();
}

bool Power::restart()
{
    QDBusReply<void> reply;
    if (m_interface->isValid())
        reply = m_interface->call("Reboot", false);
    else
        reply = m_ckInterface->call("Restart");
    return reply.isValid();
}

}
