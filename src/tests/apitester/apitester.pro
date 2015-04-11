TEMPLATE = app
TARGET = microf-apitester

QT = core gui widgets quick

include(../../config.pri)

INCLUDEPATH += ../../lib
LIBS += -L../../lib -lmicrof

SOURCES += \
    main.cpp

RESOURCES += \
    res.qrc

OTHER_FILES += \
    main.qml \
    Token.qml

DISTFILES += \
    Tools.qml

