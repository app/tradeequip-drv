include ( ../../../config.pri )

TEMPLATE    = lib
LANGUAGE    = C++

TREQPATH = ../../../../tradeequip

CONFIG  += qt warn_on dll

INCLUDEPATH += $${TREQPATH}/src/lib
HEADERS     += dsp800.h
SOURCES     += dsp800.cpp

TARGET      = tedisplay-dsp800
DESTDIR     = ../../../lib
#DLLDESTDIR = /lib

DEFINES += VF_BUILD

LIBS += -L$${TREQPATH}/lib -ltradeequip
DEFINES += _CRT_SECURE_NO_DEPRECATE 
