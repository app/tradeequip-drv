include ( ../../config.pri )
TEMPLATE    = lib
LANGUAGE    = C++
TARGET      = tereader-barcode

CONFIG      += qt warn_on
win32:CONFIG	+= dll

HEADERS     = tereaderbarcode.h

SOURCES     = tereaderbarcode.cpp

TREQPATH = ../../../tradeequip

INCLUDEPATH += $${TREQPATH}/src/lib
LIBS += -L$${TREQPATH}/lib -ltradeequip

DEFINES += TRBARCODE_BUILD

DESTDIR     = ../../lib
#DLLDESTDIR	= /lib
DEFINES += _CRT_SECURE_NO_DEPRECATE 
