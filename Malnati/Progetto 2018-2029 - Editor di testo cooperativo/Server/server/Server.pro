QT -= gui
QT += core
QT += network
QT += sql
QT += widgets

CONFIG += c++17
CONFIG -= app_bundle console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        classes/model/authenticationmessage.cpp \
        classes/database/dbinterface.cpp \
        classes/model/charactermessage.cpp \
        classes/model/configurationmessage.cpp \
        classes/model/editmessage.cpp \
        classes/model/errormessage.cpp \
        classes/model/message.cpp \
        classes/database/sql_exceptions.cpp \
        classes/model/symbol.cpp \
        classes/model/usermessage.cpp \
        classes/network/document.cpp \
        classes/network/server.cpp \
        classes/network/client.cpp \
        classes/model/documentmessage.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/authenticationmessage.h \
    include/charactermessage.h \
    include/configurationmessage.h \
    include/dbinterface.h \
    include/document.h \
    include/documentmessage.h \
    include/editmessage.h \
    include/errormessage.h \
    include/message.h \
    include/server.h \
    include/client.h \
    include/sql_exceptions.h \
    include/symbol.h \
    include/usermessage.h

RESOURCES +=

