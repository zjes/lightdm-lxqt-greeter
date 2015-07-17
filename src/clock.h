#pragma once

#include <QLabel>
#include <QTimer>

class Clock : public QLabel
{
public:
    Clock();
private:
    void timeout();
private:
    QTimer m_timer;
};
