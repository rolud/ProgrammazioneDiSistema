QT += gui
QT += core
QT += network
QT += widgets
QT += printsupport



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
        classes/GUI/connectdialog.cpp \
        classes/GUI/documenthandler.cpp \
        classes/GUI/editor.cpp \
        classes/GUI/errordialog.cpp \
        classes/GUI/iconselection.cpp \
        classes/GUI/logindialog.cpp \
        classes/GUI/registerdialog.cpp \
        classes/GUI/startupdialog.cpp \
        classes/controller.cpp \
        classes/model/authenticationmessage.cpp \
        classes/model/algorithmcrdt.cpp \
        classes/model/charactermessage.cpp \
        classes/model/configurationmessage.cpp \
        classes/model/documentmessage.cpp \
        classes/model/errormessage.cpp \
        classes/model/message.cpp \
        classes/model/symbol.cpp \
        classes/model/usermessage.cpp \
        classes/network/client.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/connectdialog.h \
    include/authenticationmessage.h \
    include/charactermessage.h \
    include/configurationmessage.h \
    include/controller.h \
    include/documenthandler.h \
    include/documentmessage.h \
    include/errordialog.h \
    include/errormessage.h \
    include/iconselection.h \
    include/logindialog.h \
    include/message.h \
    include/registerdialog.h \
    include/startupdialog.h \
    include/symbol.h \
    include/algorithmcrdt.h \
    include/client.h \
    include/editor.h \
    include/usermessage.h

FORMS += \
    classes/GUI/connectdialog.ui \
    classes/GUI/documenthandler.ui \
    classes/GUI/editor.ui \
    classes/GUI/errordialog.ui \
    classes/GUI/iconselection.ui \
    classes/GUI/logindialog.ui \
    classes/GUI/registerdialog.ui \
    classes/GUI/startupdialog.ui

RESOURCES += \
    Icons.qrc
