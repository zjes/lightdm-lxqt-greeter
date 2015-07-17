#include <QDateTime>
#include "clock.h"
#include "settings.h"

Clock::Clock()
{
    m_timer.setInterval(1000);
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, &Clock::timeout);
    m_timer.start();
    timeout();
}

void Clock::timeout()
{
    QDateTime time = QDateTime::currentDateTime();
    setText(time.toString(Settings::instance().clockFormat()));
}

