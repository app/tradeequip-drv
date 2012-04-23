unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
    INCLUDEPATH	+= /usr/include/tradeequip
}

win32 {
    UI_DIR = tmp\ui
    MOC_DIR = tmp\moc
    OBJECTS_DIR = obj
    INCLUDEPATH	+= \src\tradeequip\src\lib
    LIBS += -L/src/tradeequip/lib -ltradeequip
}

QMAKE_CLEAN += $(TARGET) Makefile
CONFIG  += qt warn_on thread release
LANGUAGE = C++

