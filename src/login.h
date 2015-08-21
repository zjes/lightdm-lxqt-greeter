#pragma once

#include <QFrame>
#include <QLabel>
#include <QTimer>
#include "private/greeter.h"

namespace Ui
{ class Login; }

class Login : public QFrame
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
    void showMessage(const QString & msg, priv::Greeter::Message type);
    void showPrompt(const QString & prompt, priv::Greeter::Prompt type);
    void greeterReseted();
    void selectUserSession(const QString& user);
private:
    Ui::Login    *m_ui;
    priv::Greeter m_greeter;
    QTimer        m_infoTimer;
};

