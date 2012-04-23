#include <qapplication.h>
#include <qspinbox.h>
#include <qmessagebox.h>
#include <qtranslator.h>
#include <qsplashscreen.h>
#include <qfile.h>
#include "mainform.h"
#include <tescalesbase.h>
#include <tefactory.h>
//#include <mercury.h>

QApplication *application = 0;
QTranslator *translator = 0, tr_app(0), tr_lib(0), tr_plugins(0);
QString lang="en", 
rcfile="",
username="",
userpassword="";
int debuglevel = 0, batch = 0, serialport = 1, modelnumber = 0;
TEScalesBase *unit = 0;

int setTranslator(QString lang)
{
        QString langdir;
#ifdef _Windows
        langdir = application->applicationDirPath()+"/";
#else
        langdir = "/usr/share/scales-test/";
#endif
        tr_app.load( langdir+"scales-test-"+lang+".qm",".");
        return 0;
}

void printHelp(){
 printf((const char *) QObject::tr(
"Scales utility v.1.0 build "__DATE__" "__TIME__"\n"
"(c) Leader InfoTech, Linux Center (Orenburg), Valery Grazdankin, 2003-2005\n"
"Command line options:\n"
"--port=N serial port number ( 1..max )\n"
"--price=xx.xx set price to xx.xx\n"
"--get get weight, price, summa values from scales display\n"
"--batch non GUI mode\n"
"--rc=filename file for write read result\n"
"--model=name scales model name ( mercury3x )\n" 
 ));
}

int 
parseCommandLine( int argc, char **argv )
{
    QString param, name, value;
    int i;
	char buf[2048];
    TEScalesBase *newunit;
        
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
	if (name == "--model") {
	    newunit = (TEScalesBase*) TEFactory::create( value );
	    if ( newunit && newunit->inherits("TEScalesBase") ) {
		delete unit; 
		unit = newunit;
	    }
	}
        if (name == "--port") {
	    serialport = value.toInt();
	    unit->setPortNumber( serialport );
	}
	if (name == "--help") { printHelp(); return 1; }
        if (name == "--debug") {
		debuglevel = 1;
		unit->setDebugLevel( 1 );
	}
	if (name == "--rc") rcfile=value;
	if (name == "--batch") batch = 1;
        if (name == "--price") {
    	    if ( !modelnumber ) continue;
	    if ( !value.isEmpty() ) {
		unit->open();
		unit->setPrice( value.toDouble() );
		unit->close();
	    }
	}
	if (name == "--get") {
	    if ( !modelnumber ) continue;
	    int rc = 0;
	    unit->open();
	    if ( !unit->isOpen() ) rc = 1;
	    if ( !unit->readDisplay() ) rc = 2;
	    sprintf(buf,"NAME=SCALES;RC=%i;WEIGHT=%.3f;PRICE=%.2f;SUMMA=%.2f\n", 
		    rc, unit->weight(), unit->price(), unit->summa() );
		if ( !rcfile.isEmpty() ) {
			QFile f( rcfile );
			f.open(IO_WriteOnly);
			f.writeBlock( buf, strlen(buf) );
			f.close();
		} else {
			printf(buf);
		}
	    unit->close();
	}
    }
    if ( !modelnumber  && batch ) {
	printf(( const char *) QObject::tr("unknown scales model.\n"));
        return 1;
    }
    return 0;
}


int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    MainForm w;
    QPixmap pixmap;
    
    application = &a;
    a.addLibraryPath( a.applicationDirPath() );
    unit = new TEScalesBase( serialport );
    if ( parseCommandLine( argc, argv ) ) return 1;
    a.installTranslator( &tr_app );
    // pixmap = QPixmap::fromMimeSource( "engine-splash-"+lang+".png" );
    // if ( pixmap.isNull() )
    if ( !batch ) { 
        w.ePortNum->setValue( serialport );
	unit->open();
	QObject::connect( unit, SIGNAL( deviceEvent( int ) ), &w, SLOT( display() ) );
//        unit->startPoll(500);
        w.show();
        a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
        return a.exec();
    }
     return 0;
}
