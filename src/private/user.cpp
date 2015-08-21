#include "lightdm/greeter.h"
#include "lightdm/user.h"

#include <QDebug>
#include "user.h"
#include "greeter.h"
#include "greeterimpl.h"
#include "../settings.h"

namespace priv {

Users::Users(Greeter & greeter):
    m_greeter(greeter)
{
}

const std::map<QString, Users::User> & Users::users()
{
    static std::map<QString, User> users;
    if (!users.empty())
        return users;

    int userCount = lightdm_user_list_get_length(lightdm_user_list_get_instance());
    if (userCount){
        const GList *items, *item;
        items = lightdm_user_list_get_users(lightdm_user_list_get_instance());
        for (item = items; item; item = item->next) {
            LightDMUser *ldmUser = static_cast<LightDMUser*>(item->data);

            User user;
            user.name = QString::fromUtf8(lightdm_user_get_name(ldmUser));
            user.homeDirectory = QString::fromUtf8(lightdm_user_get_home_directory(ldmUser));
            user.realName = QString::fromUtf8(lightdm_user_get_real_name(ldmUser));
            user.image = QString::fromUtf8(lightdm_user_get_image(ldmUser));
            user.background = QString::fromUtf8(lightdm_user_get_background(ldmUser));
            user.session = QString::fromUtf8(lightdm_user_get_session(ldmUser));
            user.loggedIn = lightdm_user_get_logged_in(ldmUser);
            user.uid = (quint64)lightdm_user_get_uid(ldmUser);
            user.session = QString::fromUtf8(lightdm_user_get_session(ldmUser));
            user.language = QString::fromUtf8(lightdm_user_get_language(ldmUser));
            user.layout = QString::fromUtf8(lightdm_user_get_layout(ldmUser));
            users.insert({user.name, qMove(user)});
        }
    }
    return users;
}

QString Users::hint() const
{
    QString user = QString::fromUtf8(lightdm_greeter_get_select_user_hint(m_greeter.m_pimpl->greeter()));
    if (user.isEmpty())
        return Settings::instance().lastUser();
    return user;
}

bool Users::inAuthentication() const
{ return lightdm_greeter_get_in_authentication(m_greeter.m_pimpl->greeter()); }

void Users::cancelAuthentication()
{ lightdm_greeter_cancel_authentication(m_greeter.m_pimpl->greeter()); }

void Users::authenticate(const QString& user)
{ lightdm_greeter_authenticate(m_greeter.m_pimpl->greeter(), qPrintable(user.trimmed())); }

void Users::authorize(const QString& pwd)
{ lightdm_greeter_respond(m_greeter.m_pimpl->greeter(), qPrintable(pwd.trimmed())); }

bool Users::isAuthenticated() const
{ return lightdm_greeter_get_is_authenticated(m_greeter.m_pimpl->greeter()); }

const Users::User & Users::authenticationUser() const
{
    static User def;
    QString user = QString::fromUtf8(lightdm_greeter_get_authentication_user(m_greeter.m_pimpl->greeter()));
    auto it = users().find(user);
    if (it != users().end()){
        return it->second;
    }
    return def;
}

QString Users::sharedDataDirSync(const QString &username)
{ return QString::fromUtf8(lightdm_greeter_ensure_shared_data_dir_sync(m_greeter.m_pimpl->greeter(), qPrintable(username))); }

}
