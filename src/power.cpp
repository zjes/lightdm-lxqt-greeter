#include <QDebug>
#include <QHBoxLayout>
#include <QToolButton>
#include "power.h"

Power::Power(QWidget *parent):
    QWidget(parent)
{
    setObjectName("Power");

    QHBoxLayout *lay = new QHBoxLayout;
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setMargin(0);

    if(m_power.canSuspend()){
        QToolButton *btn = new QToolButton;
        btn->setToolTip(tr("Suspend"));
        btn->setObjectName("Suspend");
        connect(btn, &QToolButton::clicked, [this](){
            m_power.suspend();
        });
        lay->addWidget(btn);
    }

    if(m_power.canHibernate()){
        QToolButton *btn = new QToolButton;
        btn->setToolTip(tr("Hibernate"));
        btn->setObjectName("Hibernate");
        connect(btn, &QToolButton::clicked, [this](){
            m_power.hibernate();
        });
        lay->addWidget(btn);
    }

    if(m_power.canRestart()){
        QToolButton *btn = new QToolButton;
        btn->setToolTip(tr("Restart"));
        btn->setObjectName("Restart");
        connect(btn, &QToolButton::clicked, [this](){
            m_power.restart();
        });
        lay->addWidget(btn);
    }

    if(m_power.canShutdown()){
        QToolButton *btn = new QToolButton;
        btn->setToolTip(tr("Shutdown"));
        btn->setObjectName("Shutdown");
        connect(btn, &QToolButton::clicked, [this](){
            m_power.shutdown();
        });
        lay->addWidget(btn);
    }

    setLayout(lay);
}

