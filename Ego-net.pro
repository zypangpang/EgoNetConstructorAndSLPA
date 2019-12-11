TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        graph.cpp \
        main.cpp \
        slpa.cpp \
        src/format.cc \
        utils.cpp

HEADERS += \
    SLPA.h \
    fmt/core.h \
    fmt/format-inl.h \
    fmt/format.h \
    graph.h \ \
    utils.h
