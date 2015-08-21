#include <QDebug>
#include "settings.h"
#include "login.h"
#include "ui_login.h"
#include "private/greeter.h"
#include "settings.h"

Login::Login(QWidget *parent) :
    QFrame(parent),
    m_ui(new Ui::Login)
{
    m_ui->setupUi(this);

    m_ui->infoLbl->setVisible(false);

    m_infoTimer.setInterval(5000);
    m_infoTimer.setSingleShot(true);
    connect(&m_infoTimer, &QTimer::timeout, [this](){
        m_ui->infoLbl->setVisible(false);
    });

    connect(m_ui->loginEdt, &QLineEdit::editingFinished, this, &Login::setUser);
    connect(m_ui->pwdEdt,   &QLineEdit::returnPressed,   this, &Login::doLogin);
    connect(m_ui->loginBtn, &QPushButton::clicked,       this, &Login::doLogin);
    connect(m_ui->resetBtn, &QPushButton::clicked,       this, &Login::reset);


    connect(&m_greeter, &priv::Greeter::showMessage,            this, &Login::showMessage);
    connect(&m_greeter, &priv::Greeter::showPrompt,             this, &Login::showPrompt);
    connect(&m_greeter, &priv::Greeter::authenticationComplete, this, &Login::authenticationComplete);
    connect(&m_greeter, &priv::Greeter::reset,                  this, &Login::greeterReseted);

    styleIt();
    init();
}

Login::~Login()
{
    delete m_ui;
}

void Login::styleIt()
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(Settings::instance().panelBackground()));
    pal.setColor(QPalette::Foreground, QColor(Settings::instance().panelForeground()));
    setPalette(pal);

}

void Login::init()
{
    if (!m_greeter.connectToLightDM()) {
        qWarning() << "cannot connect to lightdm";
        return;
    }

    for(const priv::Session::Info& sess: priv::Session::sessions(priv::Session::Type::Local)){
        m_ui->sessionCmb->addItem(sess.name, QVariant::fromValue(sess));
    }

    QString lastUser = m_greeter.users().hint();
    if (lastUser.isEmpty()){
        for(const auto& user: m_greeter.users().users()){
            if (user.second.loggedIn && user.second.name == Settings::instance().lastUser())
                lastUser = user.second.name;
        }
    }

    if (lastUser.isEmpty()){
        m_ui->loginEdt->setFocus();
    } else {
        selectUserSession(lastUser);
        m_ui->loginEdt->setText(lastUser);
        m_ui->pwdEdt->setFocus();
    }
}

void Login::setUser()
{
    selectUserSession(m_ui->loginEdt->text());
}

void Login::selectUserSession(const QString& user)
{
    QString sessionHint = m_greeter.session().hint(user);
    for(int i = 0; i < m_ui->sessionCmb->count(); ++i){
        QString session = m_ui->sessionCmb->itemData(i).value<priv::Session::Info>().key;
        if (session == sessionHint){
            m_ui->sessionCmb->setCurrentIndex(i);
            break;
        }
    }
}

void Login::doLogin()
{
    if (m_greeter.users().inAuthentication())
        m_greeter.users().cancelAuthentication();

    m_greeter.users().authenticate(m_ui->loginEdt->text());
}

void Login::authenticationComplete()
{
    if (m_greeter.users().isAuthenticated()) {
        qDebug() << "Authenticated... starting session" << m_ui->sessionCmb->currentData().value<priv::Session::Info>().name;
        QString session = m_ui->sessionCmb->currentData().value<priv::Session::Info>().key;

        Settings::instance().setLastUser(m_ui->loginEdt->text());
        Settings::instance().setLastUserSession(m_ui->loginEdt->text(), session);

        m_greeter.users().sharedDataDirSync(m_ui->loginEdt->text());
        m_greeter.session().start(session);
    } else {
        showInfo("Not authenticated");
        qDebug() << "Not authenticated";
        m_ui->pwdEdt->clear();
        setUser();
    }
}

void Login::showInfo(const QString& txt)
{
    m_ui->infoLbl->setText(txt);
    m_ui->infoLbl->setVisible(true);
    m_infoTimer.start();
}

void Login::showMessage(const QString & msg, priv::Greeter::Message /*type*/)
{
    showInfo(msg);
}

void Login::showPrompt(const QString & /*prompt*/, priv::Greeter::Prompt type)
{
    if (type != priv::Greeter::Prompt::TypeSecret){
        m_ui->loginEdt->clear();
        m_ui->pwdEdt->clear();
        m_ui->loginEdt->setFocus();
    } else {
        if(!m_ui->pwdEdt->text().isEmpty()){
            m_greeter.users().authorize(m_ui->pwdEdt->text());
        } else {
            m_ui->pwdEdt->setFocus();
        }
    }
}

void Login::greeterReseted()
{
    m_ui->loginEdt->clear();
    m_ui->pwdEdt->clear();
    m_ui->loginEdt->setFocus();
}

void Login::reset()
{
    m_greeter.users().authenticate(QString::null);
    greeterReseted();
}
