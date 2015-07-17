#pragma once

#include <QSettings>

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

private:
    Settings():
        m_settings("/etc/lightdm/lightdm-lxqt-greeter.conf", QSettings::IniFormat)
    {}

    QSettings m_settings;
};
