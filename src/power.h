#pragma once

#include <QWidget>
#include "private/power.h"

class Power : public QWidget
{
    Q_OBJECT
public:
    explicit Power(QWidget *parent = 0);
private:
    priv::Power m_power;
};
