TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        graph.cpp \
        main.cpp \
        src/format.cc \
        utils.cpp

HEADERS += \
    fmt/core.h \
    fmt/format-inl.h \
    fmt/format.h \
    graph.h \ \
    utils.h
