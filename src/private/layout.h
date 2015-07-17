#pragma once
#include <QMap>
#include <QScopedPointer>

namespace pimpl
{ class X11Kbd; }


class KbdInfo
{
public:
    void append(uint group, const QString & name)
    { m_info[group] = name; }

    void setCurrentGroup(uint group)
    { m_currentGroup = group; }

    void clear()
    {
        m_info.clear();
        m_currentGroup = 0;
    }

    QString current() const
    { return m_info[m_currentGroup]; }

    const QMap<uint, QString> & info() const
    { return m_info; }
private:
    QMap<uint, QString> m_info;
    uint                m_currentGroup;
};

class Layout: public QObject
{
    Q_OBJECT
public:
    Layout();
    virtual ~Layout();

    bool init();

public:
    void keyboardChanged();
    void setCurrentGroup(uint group);
    uint calcMinimalLayoutWidth(const QFont & font);
signals:
    void layoutChanged(const QString & name);
    void capsChanged(bool on);
private:
    KbdInfo                       m_info;
    QScopedPointer<pimpl::X11Kbd> m_pimpl;
};

