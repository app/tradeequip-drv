#include "testscanner.h"
#include "testscannerui.h"
#include <tereaderbarcode.h>

int main(int argc, char * argv [])
{
    QApplication app(argc,argv);
    cout << "Starting test..." << endl;
    TEReaderBase * sc=new TEReaderBarcode;
    Form1 ui;
    app.setMainWidget(&ui);
    ui.setScanner(sc);
    ui.connect(sc,SIGNAL(deviceEvent(int)),&ui,SLOT(onDevEvent(int)));
    ui.show();
    sc->setDebugLevel(1);
    sc->setPortNumber(1);
    sc->setPortBaudRate(9600);
//    sc->setPortFlowControl(FLOW_HARDWARE);
    sc->setPortFlowControl(FLOW_OFF);
    sc->setPortDataBits(DATA_8);
    sc->setPortParity(PAR_NONE);
    sc->setPortStopBits(STOP_1);
    sc->startDriver();
    cout << "Press Ctrl+C to stop test..." << endl;
    app.exec();
    sc->stopDriver();
    delete sc;
    return 0;
}
