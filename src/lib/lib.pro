TEMPLATE = lib
TARGET = microf

QT = core network core-private

CONFIG += staticlib

include(../config.pri)

HEADERS += \
    socialnetwork.h \
    socialnetwork_p.h \
    socialnetworkstatus.h \
    socialnetworkerror.h \
    socialobject.h \
    socialobject_p.h \
    socialrequest.h \
    socialrequest_p.h \
    socialcontentitem.h \
    socialcontentitem_p.h \
    socialcontentitembuilder.h \
    socialcontentitembuilder_p.h \
    facebook/facebook.h \
    facebook/facebooklogoutrequest.h \
    facebook/facebookloginrequest.h \
    facebook/facebooklogincontentbuilder.h \
    facebook/facebookconfirmationcontentbuilder.h

SOURCES += \
    socialnetwork.cpp \
    socialnetworkstatus.cpp \
    socialnetworkerror.cpp \
    socialobject.cpp \
    socialrequest.cpp \
    socialcontentitem.cpp \
    socialcontentitembuilder.cpp \
    facebook/facebook.cpp \
    facebook/facebooklogoutrequest.cpp \
    facebook/facebookloginrequest.cpp \
    facebook/facebooklogincontentbuilder.cpp \
    facebook/facebookconfirmationcontentbuilder.cpp
