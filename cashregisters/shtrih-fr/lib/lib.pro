TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= qt warn_on dll

LIBS	+= -L../../../../tradeequip/lib -ltradeequip

DEFINES	+= SHTRIH_BUILD

INCLUDEPATH	+= ../../../../tradeequip/src/lib

HEADERS	+= shtrih-fr.h

SOURCES	+= shtrih-fr.cpp

include ( ../../../config.pri )
TARGET      = tecashregister-shtrih-fr
DESTDIR     = ../../../lib
#DLLDESTDIR	= /lib

TRANSLATIONS = \
    ../../../translations/tecashregister-shtrih-fr-en.ts \
    ../../../translations/tecashregister-shtrih-fr-ru.ts 
DEFINES += _CRT_SECURE_NO_DEPRECATE 
