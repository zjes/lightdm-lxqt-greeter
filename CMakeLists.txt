cmake_minimum_required(VERSION 2.8.11)
cmake_policy(VERSION 2.8)

project(lightdm-lxqt-greeter)

find_package(Qt5 COMPONENTS Core Widgets DBus Xml X11Extras)

find_package(PkgConfig)
find_package(lxqt)
find_package(KF5WindowSystem)

pkg_check_modules(LIGHTDM        REQUIRED liblightdm-gobject-1)
pkg_check_modules(GOBJECT        REQUIRED gobject-2.0)
pkg_check_modules(XKB_COMMON_X11 REQUIRED xkbcommon-x11)
pkg_check_modules(XCB_XCB        REQUIRED xcb-xkb)

#include(${LXQT_USE_FILE})
include_directories(${LIGHTDM_INCLUDE_DIRS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_definitions("-g -fPIC -Wall -Wextra -pedantic -Wformat=2 -Wnon-virtual-dtor -std=c++11 -Wl,--as-needed -z relro -z defs")

qt5_wrap_ui(UIC src/login.ui)

add_executable(${PROJECT_NAME}
    main.cpp
    src/settings.h
    src/window.h
    src/window.cpp
    src/clock.h
    src/clock.cpp
    src/keyboard.h
    src/keyboard.cpp
    src/infopanel.h
    src/infopanel.cpp
    src/login.h
    src/login.cpp
    src/power.h
    src/power.cpp
    src/private/layout.h
    src/private/layout.cpp
    src/private/greeter.h
    src/private/greeterimpl.h
    src/private/greeter.cpp
    src/private/user.h
    src/private/user.cpp
    src/private/session.h
    src/private/session.cpp
    src/private/power.h
    src/private/power.cpp
    ${UIC}
)

target_link_libraries(${PROJECT_NAME}
    ${LIGHTDM_LIBRARIES}
    ${GOBJECT_LIBRARIES}
    ${XKB_COMMON_X11_LIBRARIES}
    ${XCB_XCB_LIBRARIES}
    ${LXQT_LIBRARIES}
    lxqt
)

qt5_use_modules(${PROJECT_NAME} Widgets)
