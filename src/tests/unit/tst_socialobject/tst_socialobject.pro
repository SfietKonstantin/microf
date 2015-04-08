TEMPLATE = app
TARGET = tst_socialobject

QT = core testlib

include(../../../config.pri)

INCLUDEPATH += ../../../lib
LIBS += -L../../../lib -lmicrof

SOURCES += tst_socialobject.cpp
