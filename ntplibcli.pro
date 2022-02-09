QT -= gui
QT += core network

TARGET = NtpLibCli
TEMPLATE = lib
DEFINES += NTPLIBCLI_LIBRARY

CONFIG += console
CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    ntplibcli.cpp \
    reply.cpp

HEADERS += \
    client.h \
    config.h \
    ntplibcli.h \
    options.h \
    packet.h \
    reply.h \
    replydata.h \
    tdist.h \
    timestamp.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
