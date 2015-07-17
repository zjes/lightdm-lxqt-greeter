#include <QDebug>
#include "settings.h"
#include "login.h"
#include "ui_login.h"
#include "private/greeter.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Login)
{
    m_ui->setupUi(this);

    m_ui->infoLbl->setVisible(false);
    m_ui->pwdEdt->setEnabled(false);

    m_infoTimer.setInterval(5000);
    m_infoTimer.setSingleShot(true);
    connect(&m_infoTimer, &QTimer::timeout, [this](){
        m_ui->infoLbl->setVisible(false);
    });

    connect(m_ui->loginEdt, &QLineEdit::editingFinished, this, &Login::setUser);
    connect(m_ui->pwdEdt,   &QLineEdit::editingFinished, this, &Login::doLogin);
    connect(m_ui->loginBtn, &QPushButton::clicked,       this, &Login::doLogin);
    connect(m_ui->resetBtn, &QPushButton::clicked,       this, &Login::reset);


    connect(&m_greeter, &Greeter::showMessage,            this, &Login::showMessage);
    connect(&m_greeter, &Greeter::showPrompt,             this, &Login::showPrompt);
    connect(&m_greeter, &Greeter::authenticationComplete, this, &Login::authenticationComplete);
    connect(&m_greeter, &Greeter::reset,                  this, &Login::greeterReseted);

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

    QString sessionHint = m_greeter.session().hint();
    int index = 0;
    for(const Session::Info& sess: Session::sessions(Session::Type::Local)){
        m_ui->sessionCmb->addItem(sess.name, QVariant::fromValue(sess));
        if (!sessionHint.isEmpty() && sess.key == sessionHint){
            m_ui->sessionCmb->setCurrentIndex(index);
        }
        ++index;
    }

    QString lastUser = m_greeter.users().selectUserHint();
    if (lastUser.isEmpty()){
        m_ui->loginEdt->setFocus();
    } else {
        m_ui->loginEdt->setText(lastUser);
        setUser();
    }
}

void Login::setUser()
{
    if (m_greeter.users().inAuthentication())
        m_greeter.users().cancelAuthentication();

    m_greeter.users().authenticate(m_ui->loginEdt->text());
    m_ui->loginEdt->setEnabled(false);
}

void Login::doLogin()
{
    m_greeter.users().authorize(m_ui->pwdEdt->text());
    m_ui->pwdEdt->clear();
}

void Login::authenticationComplete()
{
    if (m_greeter.users().isAuthenticated()) {
        qDebug() << "Authenticated... starting session" << m_ui->sessionCmb->currentData().value<Session::Info>().name;
        m_greeter.users().sharedDataDirSync(m_ui->loginEdt->text());
        m_greeter.session().start(m_ui->sessionCmb->currentData().value<Session::Info>().key);
    } else {
        showInfo("Not authenticated");
        m_ui->pwdEdt->clear();
        m_ui->pwdEdt->setEnabled(false);
        m_ui->loginEdt->setEnabled(true);
        setUser();
    }
}

void Login::showInfo(const QString& txt)
{
    m_ui->infoLbl->setText(txt);
    m_ui->infoLbl->setVisible(true);
    m_infoTimer.start();
}

void Login::showMessage(const QString & msg, Greeter::Message /*type*/)
{
    showInfo(msg);
}

void Login::showPrompt(const QString & /*prompt*/, Greeter::Prompt type)
{
    if (type != Greeter::Prompt::TypeSecret){
        m_ui->loginEdt->setText("");
        m_ui->loginEdt->setEnabled(true);
        m_ui->loginEdt->setFocus();
    } else {
        m_ui->pwdEdt->setEnabled(true);
        m_ui->pwdEdt->setFocus();
    }
}

void Login::greeterReseted()
{
    m_ui->loginEdt->setText("");
    m_ui->pwdEdt->setText("");
    m_ui->loginEdt->setEnabled(true);
    m_ui->loginEdt->setFocus();
    m_ui->pwdEdt->setEnabled(false);
}

void Login::reset()
{
    m_greeter.users().authenticate(QString::null);
    greeterReseted();
}
