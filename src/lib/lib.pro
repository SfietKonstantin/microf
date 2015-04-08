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
    socialrequest.h \
    socialobject.h \
    socialrequest_p.h \
    ipropertiesadaptor.h \
    propertiesadaptor.h \
    socialcontentbuilder.h \
    socialcontentbuilder_p.h \
    socialobject_p.h

SOURCES += \
    socialnetwork.cpp \
    socialcontentitem.cpp \
    socialrequest.cpp \
    socialobject.cpp \
    propertiesadaptor.cpp \
    socialcontentbuilder.cpp
    
