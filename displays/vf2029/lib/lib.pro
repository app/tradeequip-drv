include ( ../../../config.pri )

TEMPLATE    = lib
LANGUAGE    = C++

TREQPATH = ../../../../tradeequip

CONFIG  += qt warn_on dll

INCLUDEPATH += $${TREQPATH}/src/lib
HEADERS     += vf2029.h
SOURCES     += vf2029.cpp

TARGET      = tedisplay-vf2029
DESTDIR     = ../../../lib
#DLLDESTDIR = /lib

DEFINES += VF_BUILD

LIBS += -L$${TREQPATH}/lib -ltradeequip
DEFINES += _CRT_SECURE_NO_DEPRECATE 
