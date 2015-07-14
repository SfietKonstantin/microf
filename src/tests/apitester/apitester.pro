TEMPLATE = app
TARGET = microf-apitester

QT = core gui widgets qml quick

include(../../config.pri)

INCLUDEPATH += ../../lib
LIBS += -L../../lib -lmicrof

HEADERS += \
    authhelper.h \
    infohelper.h \
    requesthelpermodel.h \
    customfacebookrequest.h \
    buildershelpermodel.h \
    metapropertyhelpermodel.h \
    customfacebookmodelbuilder.h \
    facebookitemrawdataproxybuilder.h \
    facebookmodelrawdataproxybuilder.h

SOURCES += \
    main.cpp \
    authhelper.cpp \
    infohelper.cpp \
    requesthelpermodel.cpp \
    customfacebookrequest.cpp \
    buildershelpermodel.cpp \
    metapropertyhelpermodel.cpp \
    customfacebookmodelbuilder.cpp \
    facebookitemrawdataproxybuilder.cpp \
    facebookmodelrawdataproxybuilder.cpp

RESOURCES += \
    res.qrc

OTHER_FILES += \
    main.qml \
    Token.qml \
    Query.qml \
    Tools.qml \
    Display.qml

DISTFILES += \
    MetaEditor.qml

