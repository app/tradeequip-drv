include ( ../../config.pri )

TEMPLATE    = app
LANGUAGE    = C++

CONFIG  += qt warn_on console

TREQPATH = ../../../tradeequip

INCLUDEPATH += $${TREQPATH}/src/lib
LIBS += -L$${TREQPATH}/lib -ltradeequip


LIBS    += -L../../lib -ltradeequip -ltereader-barcode

INCLUDEPATH += ../../../include ../../lib ../../../tserialport ../barcode ../card

HEADERS += testscanner.h

SOURCES += testscanner.cpp

FORMS   = testscannerui.ui

TARGET      = tereader-test
DESTDIR     = ../../bin

TRANSLATIONS = \
    ../../translations/tereader-test-en.ts \
    ../../translations/tereader-test-ru.ts 
DEFINES += _CRT_SECURE_NO_DEPRECATE 
