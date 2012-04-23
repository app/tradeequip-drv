#include "pc.h"
#include <qthread.h>
#include <qtimer.h>

QTextCodec * TEDisplayPC::ibm866=QTextCodec::codecForName("IBM866");

/*!
 * TEDisplayPC is a driver class for customer displays PC based using 
 * tedisplay-pc application. 
 * TEDisplayPC(int pn) - class constructor.
 * pn is a com port number.
 */
TEDisplayPC::TEDisplayPC(int pn): TEDisplayBase(pn),
	updInterval(100),bBrightnessPending(false),bLoadCharsPending(false),
	bInitPending(true)
{
	setName("TEDisplayPC");
	iInitCounter=200;
	iBrightness=255;
	m_baudRate=9600;
	initPort();
	QTimer::singleShot(updInterval,this,SLOT(onTimer()));

	m_ee.addProcBinding3<TEDisplayPC,const QString &,const QString &,const QString &>(
        this,&TEDisplayPC::showText,"showText");
	
};


/*!
 * All destruction is already done in superclasses.
 */
TEDisplayPC::~TEDisplayPC()
{
};


/*!
 * initPort() - initialization of com port.
 */
void TEDisplayPC::initPort()
{
    TSerialPort *p = port();
	if ( !p ) return;
	if (!isOpen())
		open();
	p->setFlowControl(FLOW_OFF);
	setPortBaudRate(m_baudRate);
	p->setDataBits(DATA_8);
	p->setParity(PAR_NONE);
	p->setStopBits(STOP_1);
	setTimeout(100);
};



/*!
 *
 */
QString TEDisplayPC::modelInfo()
{
	return "VF2029";
};



/*!
 *
 */
void TEDisplayPC::initDisplay()
{
	bInitPending=true;
};

/*!
 *
 */
void TEDisplayPC::initDisplayInternal()
{
	QCString initstring="\x1b\x40\x1b\x52\x0c\x1b\x74\x06\x1b\x25\x60\0x0c";
	writeBlockSlow(initstring.data(),initstring.length());
	bInitPending=false;
};



/*!
 *
 */
void TEDisplayPC::setDisplayMode(const QString & mode)
{
	QStringList parms=QStringList::split(QChar(';'),mode);
	QStringList::iterator i;
	for(i=parms.begin();i!=parms.end();i++)
	{
		QStringList comval=QStringList::split(QChar('='),*i);
		if (comval.size()==2 && comval[0]=="brightness")
		{
			setBrightness(comval[1].toInt());
		}
	};
};

/*!
 *
 */
void TEDisplayPC::setBrightnessInternal()
{
	char brcommand[3]={0x1b,0x2a,iBrightness*4/256+1};
	writeBlockSlow(brcommand,3);
	bBrightnessPending=false;
};

/*!
 *
 */
void TEDisplayPC::setUpdateInterval(int interval)
{
	if (updInterval==interval)
		return;
	updInterval=interval;
	if (updInterval<10)
	    updInterval=10;
	else if (updInterval>500)
		updInterval=500;
};

/*!
 *
 */
void TEDisplayPC::setBrightness(int iBr)
{
	if (iBr<0 || iBr>255)
		return;
	iBrightness=iBr;
	bBrightnessPending=true;
};


/*!
 *
 */
void TEDisplayPC::showText(const QString & text, const QString & category, 
		const QString & mode)
{
	SEffect eff;
	if (mode=="scroll-left" || mode.left(2)=="sl")
		eff=se_scrollleft;
	else if (mode=="scroll-right" || mode.left(2)=="sr")
		eff=se_scrollright;
	else if (mode.left(1)=="w")
		eff=se_wobble;
	else
		eff=se_scrollleft;
	if (category=="line2")
	{
		l2=EString(text,displayWidth());
		l2.setEffect(eff);
	}
	else if (category=="full")
	{
		l1=EString(text,displayWidth()*2);
		l1.setEffect(eff);
	}
	else
	{
		l1=EString(text,displayWidth());
		l1.setEffect(eff);
	}
};



/*!
 *
 */
void TEDisplayPC::processEvent( int /*Code*/ )
{
};



/*! /en There's no need in implementing execCommand here, it's implemented in TEBase already.
 *
 */
/*
QStringList TEDisplayPC::execCommand(const QString & cmd)
{
	QStringList res;
	return res;
};
*/

/*!
 *
 */
int TEDisplayPC::displayWidth()
{
	return 20;
};



/*!
 *
 */
void 
TEDisplayPC::updateDisplay()
{
	l1.update();
	l2.update();
	char gohome[1]={0x0b};
	writeBlockSlow(gohome,1);
	writeBlockSlow( utf8ToDevice( l1.current() ).data(), l1.width() );
	if(l1.width()==displayWidth()) // if first line is not in fullscreen mode
		writeBlockSlow( utf8ToDevice( l2.current() ).data(), l2.width() );
};

/*!
 *
 */
void TEDisplayPC::loadUserDefChars(int _firstChar, int _lastChar, const QByteArray & arr)
{
	firstChar=_firstChar;
	lastChar=_lastChar;
	charBitmaps=arr;
	bLoadCharsPending=true;
};

/*!
 *
 */
void TEDisplayPC::loadUserDefCharsInternal()
{
	char loadchars[8]={0x1b,0x3F,0x1,0x1b,0x26,0x1,firstChar,lastChar};
	writeBlockSlow(loadchars,8);
	writeBlockSlow(charBitmaps.data(),charBitmaps.size());
	char setuserchars[3]={0x1b,0x25,0x1};
	writeBlockSlow(setuserchars,3);
	bLoadCharsPending=false;
};



/*!
 *
 */
QCString 
TEDisplayPC::utf8ToDevice( const QString &text )
{
    QTextCodec *codec = QTextCodec::codecForName("IBM866");
    return codec->fromUnicode( text );
}



/*!
 *
 */
QString 
TEDisplayPC::deviceToUtf8( const QCString &text )
{
    return ibm866->toUnicode( text );
}



/*!
 *
 */
void TEDisplayPC::onTimer()
{
	iInitCounter--;
	if (iInitCounter==0)
	{
		iInitCounter=200;
		bBrightnessPending=true;
	};
	int addInterval=0;
	if (bInitPending)
		initDisplayInternal();
	if (bLoadCharsPending)
	{
		loadUserDefCharsInternal();	
		addInterval=100;
	}
	if (bBrightnessPending)
		setBrightnessInternal();
	updateDisplay();
	QTimer::singleShot(updInterval+addInterval,this,SLOT(onTimer()));
};

int TEDisplayPC::scrollSpeed()
{
	return 1000/updInterval;
};

void TEDisplayPC::setScrollSpeed(int speed)
{
	speed=speed<1?1:speed>10?10:speed;
	setUpdateInterval(1000/speed);
};

void TEDisplayPC::setPortBaudRate(int iRate)
{
	if (iRate==9600 || iRate==19200)
	{
		superclass::setPortBaudRate(iRate);
		if (iRate==19200)
		{
			setSlowPortMode(1042);
		}
		else
		{
			setSlowPortMode(0);
		};
	};
};

#include "teplugin.h"
typedef TEPlugin<TEDisplayPC> TEDisplayPCPlugin;
TE_EXPORT_PLUGIN( TEDisplayPCPlugin )
