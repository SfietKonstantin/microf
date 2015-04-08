TEMPLATE = app
TARGET = tst_socialnetwork

QT = core network qml testlib

include(../../../config.pri)
include(../../server/server.pri)

INCLUDEPATH += ../../../lib
LIBS += -L../../../lib -lmicrof

SOURCES += tst_socialnetwork.cpp
