#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include "private/greeter.h"
#include "settings.h"
#include "infopanel.h"
#include "lightdm/system.h"
#include "clock.h"
#include "power.h"
#include "keyboard.h"

InfoPanel::InfoPanel(QWidget *parent):
    QFrame(parent)
{
    setObjectName("InfoPanel");
    /*setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(Settings::instance().panelBackground()));
    pal.setColor(QPalette::Foreground, QColor(Settings::instance().panelForeground()));
    setPalette(pal);*/

    setMinimumHeight(25);

    QHBoxLayout *lay = new QHBoxLayout;

    QLabel *host = new QLabel(priv::Greeter::hostName());
    host->setObjectName("HostName");
    lay->addWidget(host);
    lay->addStretch();
    lay->addWidget(new Clock);
    lay->addStretch();
    lay->addWidget(new Power);
    lay->addWidget(new Keyboard);

    setLayout(lay);
}

