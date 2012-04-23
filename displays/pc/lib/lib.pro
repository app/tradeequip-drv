include ( ../../../config.pri )

TEMPLATE    = lib
LANGUAGE    = C++

TREQPATH = ../../../../tradeequip

CONFIG  += qt warn_on dll

INCLUDEPATH += $${TREQPATH}/src/lib
HEADERS     += pc.h
SOURCES     += pc.cpp

TARGET      = tedisplay-pc
DESTDIR     = ../../../lib
#DLLDESTDIR	= /lib

DEFINES	+= PCLIB_BUILD

LIBS += -L$${TREQPATH}/lib -ltradeequip

DEFINES += _CRT_SECURE_NO_DEPRECATE 
