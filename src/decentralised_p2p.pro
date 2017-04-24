#-------------------------------------------------
#
# Project created by QtCreator 2017-02-15T10:14:15
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = decentralised_p2p
TEMPLATE = lib
CONFIG += staticlib

win32:CONFIG(release, debug|release): LIBS += -L"C:/Program Files/OpenSSL/lib/" -llibcrypto
else:win32:CONFIG(debug, debug|release): LIBS += -L"C:/Program Files/OpenSSL/lib/" -llibcrypto
else:unix: LIBS += -L"/usr/local/lib/" -lcrypto

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../decentralised_crypt/build-decentralised_crypt-Desktop_Qt_5_8_0_MSVC2013_64bit2-Debug/release/ -ldecentralised_crypt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../decentralised_crypt/build-decentralised_crypt-Desktop_Qt_5_8_0_MSVC2013_64bit2-Debug/debug/ -ldecentralised_crypt
else:unix: LIBS += -L$$PWD/../../decentralised_crypt/build-decentralised_crypt-build-decentralised_p2p-Desktop-Debug-Debug/ -ldecentralised_crypt

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

win32:INCLUDEPATH += "C:/Program Files/OpenSSL/include/"
else:INCLUDEPATH += "/usr/local/include/"

INCLUDEPATH += $$PWD/../../decentralised_crypt/src
DEPENDPATH += $$PWD/../../decentralised_crypt/build-decentralised_crypt-Desktop_Qt_5_8_0_MSVC2013_64bit2-Debug/debug

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    decentralised_p2p.cpp \
    dc_peer.cpp

HEADERS += \
    decentralised_p2p.h \
    dc_peer.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
