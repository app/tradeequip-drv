TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on

LIBS	+= -L../../../../tradeequip/lib -L../../../lib -ltradeequip -ltecashregister-shtrih-fr

DEFINES	+= _CRT_SECURE_NO_DEPRECATE

INCLUDEPATH	+= ../lib

SOURCES	+= main.cpp

FORMS	= mainform.ui

include ( ../../../config.pri )
TARGET      = tecashregister-shtrih-fr-test
# -ltecashregister-shtrih-fr
INCLUDEPATH += ../../../../tradeequip/src/lib
DESTDIR     = ../../../bin

TRANSLATIONS = \
    ../../../translations/tecashregister-shtrih-fr-test-en.ts \
    ../../../translations/tecashregister-shtrih-fr-test-ru.ts 
