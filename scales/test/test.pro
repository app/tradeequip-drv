include ( ../../config.pri )
TEMPLATE    = app
LANGUAGE    = C++
CONFIG  += qt warn_on
#console

LIBS    += -L../../lib -L../../../tradeequip/lib -ltradeequip
INCLUDEPATH += ../../../tradeequip/src/lib
#INCLUDEPATH += ../../../include ../../lib ../lib ../mercury ../../tserialport
FORMS   = mainform.ui
SOURCES += main.cpp
TARGET      = tescales-test
DESTDIR     = ../../bin

TRANSLATIONS = \
    ../../translations/tescales-test-en.ts \
    ../../translations/tescales-test-ru.ts 
DEFINES += _CRT_SECURE_NO_DEPRECATE 
