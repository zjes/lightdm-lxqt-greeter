#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>
#include "settings.h"
#include "infopanel.h"
#include "login.h"
#include "window.h"

Window::Window(int screen, QWidget *parent):
    QWidget(parent, Qt::Window)
{
    setAutoFillBackground(true);

    QRect screenRect = QApplication::desktop()->screenGeometry(screen);
    setGeometry(screenRect);

    QString back = Settings::instance().background();
    if (back.startsWith("#")){
        QPalette pal = palette();
        pal.setColor(QPalette::Background, QColor(back));
        setPalette(pal);
    } else if (!back.isEmpty() && QFile::exists(back)){
        qDebug() << "set background";
    }

    QVBoxLayout * lay = new QVBoxLayout;

    InfoPanel *panel = new InfoPanel;
    Login     *login = new Login(this);

    QHBoxLayout *hb = new QHBoxLayout;
    hb->setContentsMargins(0, 0, 0, 0);
    hb->setMargin(0);

    hb->addStretch(2);
    hb->addWidget(login);
    hb->addStretch(2);

    lay->addWidget(panel);
    lay->addStretch();
    lay->addLayout(hb);
    lay->addStretch();

    setLayout(lay);
}

