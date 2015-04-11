TEMPLATE = lib
TARGET = microf

QT = core network core-private

CONFIG += staticlib

include(../config.pri)

HEADERS += \
    socialnetwork.h \
    socialnetwork_p.h \
    socialcontentitem.h \
    socialcontentitem_p.h \
    socialobject.h \
    socialobject_p.h \
    socialrequest.h \
    socialrequest_p.h \
    socialcontentbuilder.h \
    socialcontentbuilder_p.h \
    facebook/facebook.h \
    facebook/facebookauthrequest.h \
    facebook/facebookauthcontentbuilder.h

SOURCES += \
    socialnetwork.cpp \
    socialcontentitem.cpp \
    socialobject.cpp \
    socialrequest.cpp \
    socialcontentbuilder.cpp \
    facebook/facebook.cpp \
    facebook/facebookauthrequest.cpp \
    facebook/facebookauthcontentbuilder.cpp
