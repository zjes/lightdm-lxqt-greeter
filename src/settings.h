#pragma once

#include <QSettings>
#include <QDebug>

class Settings
{
public:
    static Settings & instance()
    {
        static Settings sett;
        return sett;
    }

    QString background() const
    { return m_settings.value("appearance/background", "#000000").toString(); }

    QString panelBackground() const
    { return m_settings.value("appearance/panel-background", "#DEDEDE").toString(); }

    QString panelForeground() const
    { return m_settings.value("appearance/panel-foreground", "#000000").toString(); }

    QString clockFormat() const
    { return m_settings.value("appearance/clock-format", "dddd, dd MMMM, hh:mm").toString(); }

    QString theme() const
    { return m_settings.value("appearance/lxqt-theme", "ambiance").toString(); }

public:
    QString lastUser() const
    { return m_settings.value("cache/last-user", "").toString(); }

    void setLastUser(const QString& user)
    {
        m_settings.setValue("cache/last-user", user);
        m_settings.sync();
        qDebug() << m_settings.status();
    }

    QString lastUserSession(const QString& user) const
    { return m_settings.value("cache/last-"+user+"-session", "").toString(); }

    void setLastUserSession(const QString& user, const QString& session)
    {
        m_settings.setValue("cache/last-"+user+"-session", session);
        m_settings.sync();
    }

private:
    Settings():
        m_settings("/etc/lightdm/lightdm-lxqt-greeter.conf", QSettings::IniFormat),
        m_cache("/etc/lightdm/lightdm-lxqt-greeter.conf", QSettings::IniFormat)
    {}

    QSettings m_settings;
    QSettings m_cache;
};
