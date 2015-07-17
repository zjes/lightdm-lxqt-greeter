#pragma once

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include "private/greeter.h"

namespace Ui
{ class Login; }

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = 0);
    ~Login();
private:
    void styleIt();
    void init();
    void setUser();
    void doLogin();
    void reset();
    void authenticationComplete();
    void showInfo(const QString& txt);
    void showMessage(const QString & msg, Greeter::Message type);
    void showPrompt(const QString & prompt, Greeter::Prompt type);
    void greeterReseted();
private:
    Ui::Login *m_ui;
    Greeter    m_greeter;
    QTimer     m_infoTimer;
};

