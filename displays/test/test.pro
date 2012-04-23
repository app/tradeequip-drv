include ( ../../config.pri )
TEMPLATE    = app
LANGUAGE    = C++
TREQPATH = ../../../tradeequip
CONFIG  += qt warn_on 
INCLUDEPATH += $${TREQPATH}/src/lib ../vf2029/lib
SOURCES	+= main.cpp
FORMS	= control.ui
TARGET      = tedisplay-test
LIBS += -L$${TREQPATH}/lib -L../../lib -ltradeequip -ltedisplay-vf2029
DESTDIR     = ../../bin

TRANSLATIONS = \
    ../../translations/tedisplay-test-en.ts \
    ../../translations/tedisplay-test-ru.ts 

DEFINES += _CRT_SECURE_NO_DEPRECATE 
