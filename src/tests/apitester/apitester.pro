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
    requestpropertyhelpermodel.h

SOURCES += \
    main.cpp \
    authhelper.cpp \
    infohelper.cpp \
    requesthelpermodel.cpp \
    requestpropertyhelpermodel.cpp

RESOURCES += \
    res.qrc

OTHER_FILES += \
    main.qml \
    Token.qml \
    Query.qml \
    Tools.qml \
    Display.qml

