TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -L/home/zzp/Downloads/libevent-2.1.8-stable/.libs -levent
SOURCES += \
        main.cpp
