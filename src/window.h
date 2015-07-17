#pragma once

#include <QWidget>

class InfoPanel;
class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(int screen, QWidget *parent = 0);
private:
    InfoPanel *m_panel;
};
