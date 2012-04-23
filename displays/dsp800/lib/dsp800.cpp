#include "dsp800.h"
#include <qthread.h>
#include <qtimer.h>

QTextCodec * TEDisplayDSP800::ibm866=QTextCodec::codecForName("IBM866");

/*!
 * TEDisplayDSP800 is a driver class for 20x2 PTC compatible customer displays. 
 * TEDisplayDSP800(int pn) - class constructor.
 * pn is a com port number.
 */
TEDisplayDSP800::TEDisplayDSP800(int pn): TEDisplayBase(pn),
  updInterval(100),bBrightnessPending(false),bLoadCharsPending(false),
  bInitPending(true)
{
  setName("TEDisplayDSP800");
  iInitCounter=200;
  iBrightness=255;
  m_baudRate=9600;
  setCommandSet(CS_DSP800);
  m_ee.addProcBinding3<TEDisplayDSP800,const QString &,const QString &,const QString &>(
        this,&TEDisplayDSP800::showText,"showText");
};


/*!
 * All destruction is already done in superclasses.
 */
TEDisplayDSP800::~TEDisplayDSP800()
{
};


/*!
 * initPort() - initialization of com port.
 */
void 
TEDisplayDSP800::initPort()
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
  setTimeout(0);
  QTimer::singleShot(updInterval,this,SLOT(onTimer()));
};



/*!
 *
 */
QString 
TEDisplayDSP800::modelInfo()
{
  return "VF2029";
};



/*!
 *
 */
void 
TEDisplayDSP800::initDisplay()
{
  bInitPending=true;
};

/*!
 *
 */
void 
TEDisplayDSP800::initDisplayInternal()
{
  QCString initstring;
  switch(m_eCommandSet)
  {
  case CS_FIRICH:
    initstring="\x1b\x40\x1b\x66R\x1b\x63R\x0c";
    break;
  case CS_AEDEX:
    break; // no init codes
  case CS_DSP800:
    initstring="\x04\x01\x25\x17\x04\x01\x49\x0c\x17";
    break;
  case CS_EPSON:
    initstring="\x1b@\x1bt\x06\x1bR\x0c\x0c";
    break;
  }
  writeBlockSlow(initstring.data(),initstring.length());
  bInitPending=false;
};



/*!
 *
 */
void 
TEDisplayDSP800::setDisplayMode(const QString & mode)
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
void 
TEDisplayDSP800::setBrightnessInternal()
{
  char brcommand[][6]=
    {
      {3,0x1b,0x2a,iBrightness*4/256+1}, //  FIRICH
      {3,0x1b,0x58,iBrightness*4/256+1}, // EPSON
      {5,0x04,0x01,0x41,iBrightness*4/256+0x31,0x17} // DSP800
    };
  switch(m_eCommandSet)
  {
  case CS_FIRICH:
    writeBlockSlow(brcommand[0],brcommand[0][0]);
    break;
  case CS_EPSON:
    writeBlockSlow(brcommand[1],brcommand[1][0]);
    break;
  case CS_DSP800:
    writeBlockSlow(brcommand[2],brcommand[2][0]);
    break;
  }
  bBrightnessPending=false;
};

/*!
 *
 */
void 
TEDisplayDSP800::setUpdateInterval(int interval)
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
void 
TEDisplayDSP800::setBrightness(int iBr)
{
  if (iBr<0 || iBr>255)
    return;
  iBrightness=iBr;
  bBrightnessPending=true;
};


/*!
 *
 */
void 
TEDisplayDSP800::showText(const QString & text, const QString & category, 
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
//  onTimer();
};



/*!
 *
 */
void 
TEDisplayDSP800::processEvent( int /*Code*/ )
{
};



/*! /en There's no need in implementing execCommand here, it's implemented in TEBase already.
 *
 */
/*
QStringList DisplayVF2029::execCommand(const QString & cmd)
{
  QStringList res;
  return res;
};
*/

/*!
 *
 */
int 
TEDisplayDSP800::displayWidth()
{
  return 20;
};



/*!
 *
 */
void 
TEDisplayDSP800::updateDisplay()
{
  l1.update();
  l2.update();
  switch(m_eCommandSet)
  {
  case CS_FIRICH:
  case CS_EPSON:
    {
      char gohome[1]={0x0b};
      writeBlockSlow(gohome,1);
    }
    break;
  case CS_DSP800:
    {
      char gohome[]={0x04,0x01,0x50,0x31,0x17};
      writeBlockSlow(gohome,sizeof(gohome));
    }
    break;
  case CS_AEDEX:
    {
      if(l1.width()==displayWidth()) // if first line is not in fullscreen mode
      {
        char gohome[]={0x21,0x23,0x31};
        writeBlockSlow(gohome,sizeof(gohome));
        writeBlockSlow( utf8ToDevice( l1.current() ).data(), l1.width() );
        char endchar[]={0x0d};
        writeBlockSlow(endchar,sizeof(endchar));
        gohome[2]=0x32;
        writeBlockSlow(gohome,sizeof(gohome));
        writeBlockSlow( utf8ToDevice( l2.current() ).data(), l2.width() );
        writeBlockSlow(endchar,sizeof(endchar));
      }
      else
      {
        char gohome[]={0x21,0x23,0x39};
        writeBlockSlow(gohome,sizeof(gohome));
        writeBlockSlow( utf8ToDevice( l1.current() ).data(), l1.width() );
        char endchar[]={0x0d};
        writeBlockSlow(endchar,sizeof(endchar));
      }
      return;
    }
    break;
  }
  writeBlockSlow( utf8ToDevice( l1.current() ).data(), l1.width() );
  if(l1.width()==displayWidth()) // if first line is not in fullscreen mode
    writeBlockSlow( utf8ToDevice( l2.current() ).data(), l2.width() );
};

/*!
 *
 */
void 
TEDisplayDSP800::loadUserDefChars(int _firstChar, int _lastChar, const QByteArray & arr)
{
  firstChar=_firstChar;
  lastChar=_lastChar;
  charBitmaps=arr;
  bLoadCharsPending=true;
};

/*!
 *
 */
void 
TEDisplayDSP800::loadUserDefCharsInternal()
{
  //char loadchars[8]={0x1b,0x3F,0x1,0x1b,0x26,0x1,firstChar,lastChar};
  //writeBlockSlow(loadchars,8);
  //writeBlockSlow(charBitmaps.data(),charBitmaps.size());
  //char setuserchars[3]={0x1b,0x25,0x1};
  //writeBlockSlow(setuserchars,3);
  bLoadCharsPending=false;
};



/*!
 *
 */
QCString 
TEDisplayDSP800::utf8ToDevice( const QString &text )
{
  return ibm866->fromUnicode(text);
}



/*!
 *
 */
QString 
TEDisplayDSP800::deviceToUtf8( const QCString &text )
{
   return ibm866->toUnicode(text);
}



/*!
 *
 */
void 
TEDisplayDSP800::onTimer()
{
  iInitCounter--;
  if (iInitCounter==0)
  {
    iInitCounter=200;
    bBrightnessPending=true;
  }
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
}

int 
TEDisplayDSP800::scrollSpeed()
{
  return 1000/updInterval;
}

void 
TEDisplayDSP800::setScrollSpeed(int speed)
{
  speed=speed<1?1:speed>10?10:speed;
  setUpdateInterval(1000/speed);
}

void 
TEDisplayDSP800::setPortBaudRate(int iRate)
{
  if (iRate==9600 || iRate==19200)
  {
    superclass::setPortBaudRate(iRate);
    if (iRate==19200)
    {
      qWarning("Trying to set slow write mode!");
      setSlowPortMode(1042);
    }
    else
    {
      setSlowPortMode(0);
    }
  }
}

/*! Sets command set for driver to use.
  Returns non-zero on error.
*/
int TEDisplayDSP800::setCommandSet(int eCS)
{
  switch(eCS)
  {
    case TEDisplayBase::CS_AEDEX:
    case TEDisplayBase::CS_DEFAULT:
    case TEDisplayBase::CS_DSP800:
    case TEDisplayBase::CS_EPSON:
    case TEDisplayBase::CS_FIRICH:
      break;
    default:
      setErrorText(tr("Mode '%1' is not supported").arg(TEDisplayBase::commandSetName(eCS)));
      return 100;
  }
  m_eCommandSet=eCS;
  return 0;
}

bool TEDisplayDSP800::isCommandSetSupported(int eCS)
{
  switch(eCS)
  {
    case TEDisplayBase::CS_AEDEX:
    case TEDisplayBase::CS_DEFAULT:
    case TEDisplayBase::CS_DSP800:
    case TEDisplayBase::CS_EPSON:
    case TEDisplayBase::CS_FIRICH:
      return true;
    default:
      return false;
  }
}


#include "teplugin.h"
typedef TEPlugin<TEDisplayDSP800> TEDisplayDSP800Plugin;
TE_EXPORT_PLUGIN( TEDisplayDSP800Plugin )
