#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
#include <LXQt/lxqtsettings.h>
#include "src/window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    const LxQt::LxQtTheme & theme = LxQt::LxQtTheme::currentTheme();
    QFile qss(theme.path()+"/lightdm-lxqt-greeter.qss");
    if(qss.exists()){
        if (qss.open(QIODevice::ReadOnly)){
            app.setStyleSheet(qss.readAll());
            qss.close();
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
