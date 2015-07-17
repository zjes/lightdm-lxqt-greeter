#include <QDebug>
#include <QHBoxLayout>
#include "keyboard.h"

Keyboard::Keyboard(QWidget *parent):
    QWidget(parent)
{
    setLayout(new QHBoxLayout);

    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->setSpacing(10);

    m_caps = new QLabel("lay");
    m_layout = new QLabel("caps");

    m_caps->setAlignment(Qt::AlignCenter);
    m_layout->setAlignment(Qt::AlignCenter);

    connect(&m_lay, &Layout::layoutChanged, [this](const QString & name){
        m_layout->setText(name);
        qDebug() << "set text" << name;
    });

    connect(&m_lay, &Layout::capsChanged, [this](bool on){
        m_caps->setText(on ? "Caps ON" : "Caps OFF");
    });

    layout()->addWidget(m_caps);
    layout()->addWidget(m_layout);

    m_lay.init();

    m_layout->setMinimumWidth(m_lay.calcMinimalLayoutWidth(font()));
}