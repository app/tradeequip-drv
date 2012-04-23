#include <qapplication.h>
#include <qmessagebox.h>
#include <qtranslator.h>
#include <qsplashscreen.h>
#include "mainform.h"
#include <tdatafile.h>
#include <qlabel.h>
#include "lsbinder.h"

QApplication *application = 0;
QTranslator *translator = 0, tr_app(0), tr_lib(0), tr_plugins(0);
QString lang="ru", 
    rcfile="",
    username="",
    userpassword="";

int setTranslator(QString lang)
{
        QString langdir;
#ifdef WIN32
    langdir = application->applicationDirPath()+"/";
#else
    langdir = "/usr/share/tedisplay-gui/";
#endif
    tr_app.load( langdir+"tedisplay-gui-"+lang+".qm",".");
    return 0;
}

int 
parseCommandLine( int argc, char **argv )
{
    QString param, name, value;
    int i;
    
    for ( i=1; i<argc; i++) {
        param = argv[i];
        name = param.section("=",0,0).lower();
        value = param.section("=",1);
//      printf("%s = %s\n", (const char *) name, (const char *) value );
        if (name == "--lang") {
        lang = value;
            setTranslator( lang );
        }
        if (name == "--rc") rcfile = value;
    }
    return 0;
}

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    MainForm w;
    QPixmap pixmap;

    application = &a;
    setTranslator("ru");
    a.addLibraryPath( a.applicationDirPath() );
    if ( parseCommandLine( argc, argv ) ) return 1;
    a.installTranslator( &tr_app );
//  pixmap = QPixmap::fromMimeSource( "engine-splash-"+lang+".png" );
//  if ( pixmap.isNull() )
	TDataFile tdf("tedisplay-gui.ini");
	TDataFileStream tds(&tdf);
	tdf.openRead();
	tdf.useSection("Params");
	w.m_Serializer.LoadSettings(&tds);
	tdf.close();

    w.showFullScreen();
//    int wdt=w.width();
//    int hgt=w.height();
//    w.setMinimumWidth(wdt);
//    w.setMaximumWidth(wdt);
//    w.setMinimumHeight(hgt);
//    w.setMaximumHeight(hgt);
//    w.fixProductNameSize();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    int ret=a.exec();
	tdf.openWrite();
	tdf.writeSection("Params");
	w.m_Serializer.SaveSettings(&tds);
	tdf.close();
	return ret;
}
