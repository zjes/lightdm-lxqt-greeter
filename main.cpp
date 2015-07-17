#include <QApplication>
#include <QDesktopWidget>
#include "src/window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //for (int i = 0; i < QApplication::desktop()->screenCount(); ++i){
        Window * wnd = new Window(0);
        wnd->show();
    //}
    //wnd->setFocus(Qt::OtherFocusReason);
    wnd->activateWindow();
    return app.exec();
}
