#include <vf2029.h>
#include <qapplication.h>
#include <qlineedit.h>
#include <qcstring.h>
#include "control.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP1251"));
    QWidget * mw=new Form1(0);
    app.setMainWidget(mw);
    TEDisplayVF2029 * disp=new TEDisplayVF2029(1);
    disp->initDisplay();
    disp->setUpdateInterval(1000);
    disp->showText("Дисплей покупателя VF2029. Внимание!!! Не выключать во время работы!");
    disp->showText("Кол-во:1 Цена: $1000.00 Скидка: $0.99","line2");
    mw->connect(mw,SIGNAL(textChanged(const QString &,const QString &,const QString &)),
        disp,SLOT(showText(const QString &,const QString &,const QString &)));
    mw->connect(mw,SIGNAL(brightnessChanged(int)),
        disp,SLOT(setBrightness(int)));
    mw->connect(mw,SIGNAL(updateIntervalChanged(int)),
        disp,SLOT(setUpdateInterval(int)));
    mw->connect(mw,SIGNAL(uploadChars(int,int,const QByteArray&)),
        disp,SLOT(loadUserDefChars(int,int,const QByteArray&)));
    mw->connect(mw,SIGNAL(initDisplay()),disp,SLOT(initDisplay()));
    mw->connect(mw,SIGNAL(baudRateChanged(int)),disp,SLOT(setPortBaudRate(int)));
    mw->connect(mw,SIGNAL(comPortNumChanged(int)),disp,SLOT(setPortNumberAndInit(int)));
    mw->show();
    return app.exec();
};
