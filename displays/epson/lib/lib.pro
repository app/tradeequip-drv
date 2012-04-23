include ( ../../../config.pri )

TEMPLATE    = lib
LANGUAGE    = C++

TREQPATH = ../../../../tradeequip

CONFIG  += qt warn_on dll

INCLUDEPATH += $${TREQPATH}/src/lib
HEADERS     += epson.h
SOURCES     += epson.cpp

TARGET      = tedisplay-epson
DESTDIR     = ../../../lib
#DLLDESTDIR	= /lib

DEFINES	+= EPSON_BUILD

LIBS += -L$${TREQPATH}/lib -ltradeequip
DEFINES += _CRT_SECURE_NO_DEPRECATE 
