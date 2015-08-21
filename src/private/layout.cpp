#include <QCoreApplication>
#include <QAbstractNativeEventFilter>
#include <QDebug>
#include <QFile>
#include <QFontMetrics>

#include <xkbcommon/xkbcommon-x11.h>
#include <xcb/xcb.h>
#define explicit _explicit
#include <xcb/xkb.h>

#include "layout.h"

namespace priv { namespace pimpl {

class X11Kbd: public QAbstractNativeEventFilter
{
public:
    X11Kbd(Layout *pub):
        m_pub(pub)
    {}

    bool init()
    {
        m_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
        m_connection = xcb_connect(0, 0);

        if (!m_connection || xcb_connection_has_error(m_connection)){
            qWarning() << "Couldn't connect to X server: error code"
                       << (m_connection ? xcb_connection_has_error(m_connection) : -1);
            return false;
        }

        xkb_x11_setup_xkb_extension(m_connection,
            XKB_X11_MIN_MAJOR_XKB_VERSION,
            XKB_X11_MIN_MINOR_XKB_VERSION,
            XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS,
            NULL, NULL, &m_eventType, NULL
        );

        m_deviceId = xkb_x11_get_core_keyboard_device_id(m_connection);
        qApp->installNativeEventFilter(this);

        readState();
        return true;
    }

    virtual ~X11Kbd()
    {
        xkb_state_unref(m_state);
        xkb_keymap_unref(m_keymap);
        xcb_disconnect(m_connection);
        xkb_context_unref(m_context);
    }

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *)
    {
        if (eventType != "xcb_generic_event_t")
            return false;

        xcb_generic_event_t *event = static_cast<xcb_generic_event_t*>(message);
        if ((event->response_type & ~0x80) == m_eventType){
            xcb_xkb_state_notify_event_t *sevent = reinterpret_cast<xcb_xkb_state_notify_event_t*>(event);
            switch(sevent->xkbType){
            case XCB_XKB_STATE_NOTIFY:
                xkb_state_update_mask(m_state,
                    sevent->baseMods,
                    sevent->latchedMods,
                    sevent->lockedMods,
                    sevent->baseGroup,
                    sevent->latchedGroup,
                    sevent->lockedGroup
                );

                if(sevent->changed & XCB_XKB_STATE_PART_GROUP_STATE){
                    m_pub->setCurrentGroup(sevent->group);
                }

                if(sevent->changed & XCB_XKB_STATE_PART_MODIFIER_LOCK){
                    bool oldState = m_isCaps;
                    bool newState = xkb_state_led_name_is_active(m_state, XKB_LED_NAME_CAPS);
                    if(oldState != newState){
                        m_isCaps = newState;
                        emit m_pub->capsChanged(newState);
                    }
                }
                break;
            case XCB_XKB_NEW_KEYBOARD_NOTIFY:
                readState();
                break;
            }
        }
        return false;
    }

    void readKbdInfo(KbdInfo & info) const
    {
        info.clear();
        xkb_layout_index_t count = xkb_keymap_num_layouts(m_keymap);
        for(xkb_layout_index_t i = 0; i < count; ++i){
            QString name = xkb_keymap_layout_get_name(m_keymap, i);
            info.append(i, name);
            if (xkb_state_layout_index_is_active(m_state, i, XKB_STATE_LAYOUT_EFFECTIVE))
                info.setCurrentGroup(i);
        }
    }

    bool isCapsOn() const
    { return m_isCaps; }

private:
    void readState()
    {
        if (m_keymap)
            xkb_keymap_unref(m_keymap);
        m_keymap = xkb_x11_keymap_new_from_device(m_context, m_connection, m_deviceId, (xkb_keymap_compile_flags)0);

        if (m_state)
            xkb_state_unref(m_state);
        m_state = xkb_x11_state_new_from_device(m_keymap, m_connection, m_deviceId);

        m_isCaps = xkb_state_led_name_is_active(m_state, XKB_LED_NAME_CAPS);
        m_pub->keyboardChanged();
    }

private:
    struct xkb_context *m_context    = 0;
    xcb_connection_t   *m_connection = 0;
    int32_t             m_deviceId;
    uint8_t             m_eventType;
    xkb_state          *m_state      = 0;
    xkb_keymap         *m_keymap     = 0;
    Layout             *m_pub;
    bool                m_isCaps;
};

}


Layout::Layout():
    m_pimpl(new pimpl::X11Kbd(this))
{
}

Layout::~Layout()
{}

bool Layout::init()
{
    return m_pimpl->init();
}

void Layout::keyboardChanged()
{
    m_pimpl->readKbdInfo(m_info);
    emit layoutChanged(m_info.current());
    emit capsChanged(m_pimpl->isCapsOn());
}

void Layout::setCurrentGroup(uint group)
{
    m_info.setCurrentGroup(group);
    emit layoutChanged(m_info.current());
}

uint Layout::calcMinimalLayoutWidth(const QFont & font)
{
    uint max = 0;
    QFontMetrics met(font);
    for(const QString & name : m_info.info()){
        uint w = met.width(name);
        if (w > max)
            max = w;
    }
    return max;
}

}
