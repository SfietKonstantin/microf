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
    isocialcontent.h \
    abstractsocialcontent_p.h \
    socialcontentitem.h \
    socialcontentitem_p.h \
    socialcontentitembuilder.h \
    socialcontentitembuilder_p.h \
    socialcontentmodel.h \
    socialcontentmodel_p.h \
    socialcontentmodelbuilder.h \
    socialcontentmodelbuilder_p.h \
    facebook/facebook.h \
    facebook/facebook_p.h \
    facebook/abstractfacebookrequest.h \
    facebook/facebookloginrequest.h \
    facebook/facebooklogincontentbuilder.h \
    facebook/facebooklogoutrequest.h \
    facebook/facebookconfirmationcontentbuilder.h \
    facebook/facebookproperty.h \
    facebook/facebookproperty_p.h \
    facebook/facebooklistproperty.h \
    facebook/facebookitembuilder.h \
    facebook/abstractfacebookmodelbuilder.h \
    facebook/abstractfacebookmodelbuilder_p.h \
    facebook/facebookmodelbuilder.h \
    facebook/facebookfriendlistrequest.h \
    facebook/facebookusersummaryrequest.h \
    facebook/facebooknewsfeedrequest.h \
    facebook/facebooknewsfeedmodelbuilder.h \
    plugin.h

SOURCES += \
    socialnetwork.cpp \
    socialnetworkstatus.cpp \
    socialnetworkerror.cpp \
    socialobject.cpp \
    socialrequest.cpp \
    socialcontentitem.cpp \
    socialcontentitembuilder.cpp \
    socialcontentmodel.cpp \
    socialcontentmodelbuilder.cpp \
    facebook/facebook.cpp \
    facebook/abstractfacebookrequest.cpp \
    facebook/facebookloginrequest.cpp \
    facebook/facebooklogincontentbuilder.cpp \
    facebook/facebooklogoutrequest.cpp \
    facebook/facebookconfirmationcontentbuilder.cpp \
    facebook/facebookproperty.cpp \
    facebook/facebooklistproperty.cpp \
    facebook/facebookitembuilder.cpp \
    facebook/abstractfacebookmodelbuilder.cpp \
    facebook/facebookmodelbuilder.cpp \
    facebook/facebookfriendlistrequest.cpp \
    facebook/facebookusersummaryrequest.cpp \
    facebook/facebooknewsfeedrequest.cpp \
    facebook/facebooknewsfeedmodelbuilder.cpp
