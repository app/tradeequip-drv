TEMPLATE	= app
LANGUAGE	= C++

TREQPATH = ../../../../tradeequip

LIBS	+= -L$${TREQPATH}/lib -ltradeequip

DEFINES	+= TRADEEQUIP_EXPORT

INCLUDEPATH	+= $${TREQPATH}/src/lib ../lib

HEADERS	+= lsbinder.h

SOURCES	+= main.cpp \
	lsbinder.cpp

FORMS	= mainform.ui \
	dconfig.ui

IMAGES	= logo.bmp

include ( ../../../config.pri )
TARGET = tedisplay-gui

TRANSLATIONS = \
    ../../../translations/tedisplay-gui-en.ts \
    ../../../translations/tedisplay-gui-ru.ts 


#CONFIG	+= qt warn_on console
#
DEFINES	+= TRADEEQUIP_EXPORT 

DEFINES += _CRT_SECURE_NO_DEPRECATE 
