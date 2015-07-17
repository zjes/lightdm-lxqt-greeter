#pragma once

#include "lightdm/greeter.h"
#include "greeter.h"

namespace pimpl {

class Greeter
{
public:
    typedef ::Greeter Public;
    Greeter(Public *pub):
        m_pub(pub)
    {
        m_ldmGreeter = lightdm_greeter_new();

        g_signal_connect(m_ldmGreeter, LIGHTDM_GREETER_SIGNAL_SHOW_PROMPT, G_CALLBACK(showPrompt), m_pub);
        g_signal_connect(m_ldmGreeter, LIGHTDM_GREETER_SIGNAL_SHOW_MESSAGE, G_CALLBACK(showMessage), m_pub);
        g_signal_connect(m_ldmGreeter, LIGHTDM_GREETER_SIGNAL_AUTHENTICATION_COMPLETE, G_CALLBACK(authenticationComplete), m_pub);
        g_signal_connect(m_ldmGreeter, LIGHTDM_GREETER_SIGNAL_RESET, G_CALLBACK(reset), m_pub);
    }

    ~Greeter()
    {
        delete m_ldmGreeter;
    }

    static void showPrompt(LightDMGreeter *, const gchar *text, LightDMPromptType type, gpointer data)
    {
        emit static_cast<Public*>(data)->showPrompt(
            QString::fromUtf8(text),
            type == LIGHTDM_PROMPT_TYPE_QUESTION ? Public::Prompt::Question : Public::Prompt::TypeSecret
        );
    }

    static void showMessage(LightDMGreeter *, const gchar *text, LightDMMessageType type, gpointer data)
    {
        emit static_cast<Public*>(data)->showMessage(
            QString::fromUtf8(text),
            type == LIGHTDM_MESSAGE_TYPE_INFO ? Public::Message::Info : Public::Message::Error
        );
    }

    static void authenticationComplete(LightDMGreeter *, gpointer data)
    {
        emit static_cast<Public*>(data)->authenticationComplete();
    }

    static void reset(LightDMGreeter *, gpointer data)
    {
        emit static_cast<Public*>(data)->reset();
    }

    LightDMGreeter *greeter() const
    { return m_ldmGreeter; }
private:
    LightDMGreeter *m_ldmGreeter;
    ::Greeter      *m_pub;
};

}
