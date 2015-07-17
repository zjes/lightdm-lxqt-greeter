#pragma once

#include <QLabel>
#include "private/layout.h"

class Keyboard: public QWidget
{
    Q_OBJECT
public:
    Keyboard(QWidget *parent = 0);
private:
    Layout  m_lay;
    QLabel *m_caps;
    QLabel *m_layout;
};
