#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include "private/greeter.h"
#include "settings.h"
#include "infopanel.h"
#include "lightdm/system.h"
#include "clock.h"
#include "keyboard.h"

InfoPanel::InfoPanel(QWidget *parent):
    QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(Settings::instance().panelBackground()));
    pal.setColor(QPalette::Foreground, QColor(Settings::instance().panelForeground()));
    setPalette(pal);

    setLayout(new QHBoxLayout);
    setMinimumHeight(25);
    layout()->addWidget(new QLabel(Greeter::hostName()));
    layout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout()->addWidget(new Clock);
    layout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout()->addWidget(new Keyboard);
}

