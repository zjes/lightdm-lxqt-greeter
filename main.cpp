#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
#include <LXQt/lxqtsettings.h>
#include "src/window.h"
#include "src/settings.h"

LxQt::LxQtTheme currentTheme()
{
    QString themeName = Settings::instance().theme();
    for(const LxQt::LxQtTheme & theme: LxQt::LxQtTheme::allThemes()){
        if (theme.name() == themeName)
            return theme;
    }
    return LxQt::LxQtTheme::currentTheme();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LxQt::LxQtTheme theme = currentTheme();
    QFile qss(theme.path()+"/lightdm-lxqt-greeter.qss");
    if(qss.exists()){
        if (qss.open(QIODevice::ReadOnly)){
            QString qssCnt = qss.readAll();
            app.setStyleSheet(qssCnt);
            qss.close();
        } else {
            qDebug() << qss.errorString();
        }
    }

    //for (int i = 0; i < QApplication::desktop()->screenCount(); ++i){
        Window * wnd = new Window(0);
        wnd->show();
    //}
    //wnd->setFocus(Qt::OtherFocusReason);
    wnd->activateWindow();
    return app.exec();
}
