CONFIG += c++11
CONFIG(testing) {
    DEFINES += MICROF_DEBUG
    QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
    QMAKE_LFLAGS_DEBUG += -lgcov -coverage
}
CONFIG(microfdebug) {
    DEFINES += MICROF_DEBUG
}

