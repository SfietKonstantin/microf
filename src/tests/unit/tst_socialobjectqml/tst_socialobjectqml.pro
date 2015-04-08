TEMPLATE = app
TARGET = tst_socialobjectqml

QT = core qml
CONFIG += qmltestcase

include(../../../config.pri)

INCLUDEPATH += ../../../lib
LIBS += -L../../../lib -lmicrof

SOURCES += tst_socialobjectqml.cpp

RESOURCES += res.qrc

OTHER_FILES += tst_socialobjectqml.qml
