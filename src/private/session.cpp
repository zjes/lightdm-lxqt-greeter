#include "lightdm/session.h"
#include "greeterimpl.h"
#include "session.h"

Session::Session(Greeter & greeter):
    m_greeter(greeter)
{}

QList<Session::Info> Session::sessions(Type type)
{
    QList<Info> items;
    GList *ldmSessions;

    switch (type) {
    case Type::Remote:
        ldmSessions = lightdm_get_remote_sessions();
        break;
    case Type::Local:
        ldmSessions = lightdm_get_sessions();
        break;
    }

    for (GList* item = ldmSessions; item; item = item->next) {
        LightDMSession *ldmSession = static_cast<LightDMSession*>(item->data);

        Info session;
        session.key = QString::fromUtf8(lightdm_session_get_key(ldmSession));
        session.type = QString::fromUtf8(lightdm_session_get_session_type(ldmSession));
        session.name = QString::fromUtf8(lightdm_session_get_name(ldmSession));
        session.comment = QString::fromUtf8(lightdm_session_get_comment(ldmSession));

        items.append(qMove(session));
    }
    return items;
}

bool Session::start(const QString &session)
{ return lightdm_greeter_start_session_sync(m_greeter.m_pimpl->greeter(), qPrintable(session), NULL); }

QString Session::hint() const
{ return QString::fromUtf8(lightdm_greeter_get_default_session_hint(m_greeter.m_pimpl->greeter())); }
