QT += \
    core\
    gui\
    charts\
    serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    pwchartview.cpp \
    qdlglogin.cpp \
    qdlglogon.cpp

HEADERS += \
    mainwindow.h \
    pwchartview.h \
    qdlglogin.h \
    qdlglogon.h

FORMS += \
    mainwindow.ui \
    qdlglogin.ui \
    qdlglogon.ui

RC_ICONS += icon/pulse.ico

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
VERSION = 23.6.3
