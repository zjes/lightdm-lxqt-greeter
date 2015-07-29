#include "lightdm/greeter.h"
#include "lightdm/system.h"
#include <QDebug>
#include "greeter.h"
#include "greeterimpl.h"

Greeter::Greeter():
    m_pimpl(new pimpl::Greeter(this)),
    m_users(*this),
    m_session(*this)
{}

Greeter::~Greeter()
{}

bool Greeter::connectToLightDM()
{
    if(lightdm_greeter_connect_to_daemon_sync(m_pimpl->greeter(), NULL)){
        lightdm_greeter_set_resettable(m_pimpl->greeter(), true);
        qDebug() << QString::fromUtf8(lightdm_greeter_get_hint(m_pimpl->greeter(), "LightDM/cache-directory"));
        return true;
    }
    return false;
}

QString Greeter::hostName()
{ return QString::fromUtf8(lightdm_get_hostname()); }
